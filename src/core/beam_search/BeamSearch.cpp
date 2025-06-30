#include "BeamSearch.h"
#include <algorithm>
#include <QDebug>
#include <QSet>
#include <QThread>

BeamSearch::BeamSearch(const Scenario& scenario, qint64 beam_size, qint64 max_time)
    : _scenario(scenario), _beam_size(beam_size), _time_penalty(1.0), _max_time(max_time)
{
    BeamState initial_state{scenario.initial_stock, 0, 0.0};
    initial_state.score = this->ComputeScore(initial_state, _scenario);
    _current_beam.append(BeamNode(0, -1, initial_state, "START"));
    _nodes_vector.append(_current_beam.first());
    _seen_stock_best_time.clear();
}

void BeamSearch::RunAlgorithm()
{
    _nodes_vector.clear();
    _current_beam.clear();
    _seen_stock_best_time.clear();

    BeamState initial_state{_scenario.initial_stock, 0, 0.0};
    initial_state.score = this->ComputeScore(initial_state, _scenario);
    BeamNode root_node(0, -1, initial_state, "START");
    _nodes_vector.append(root_node);
    _current_beam.append(root_node);

    int next_id = 1;
    int iteration = 0;

    while (!_current_beam.isEmpty()) {
        iteration++;
        
        qint64 max_beam_time = 0;
        for (const BeamNode& node : _current_beam) {
            if (node.getState().time > max_beam_time) {
                max_beam_time = node.getState().time;
            }
        }
        
        if (max_beam_time > _max_time) {
            break;
        }
        
        if (QThread::currentThread()->isInterruptionRequested()) {
            qDebug() << "Beam search interrupted.";
            break;
        }

        double max_score = std::numeric_limits<double>::lowest();
        const BeamNode* best_node = nullptr;
        for (const BeamNode& node : _current_beam) {
            if (node.score() > max_score) {
                max_score = node.score();
                best_node = &node;
            }
        }

        if (iteration % 10 == 0 || iteration <= 5) {
            if (best_node) {
                qDebug() << "Iteration" << iteration << "- Stock:" << best_node->getState().stock.toString() 
                         << "- Temps:" << best_node->getState().time;
            }
        }

        if (_score_callback)
            _score_callback(max_score);

        this->ExpandBeam(_current_beam, next_id);
    }

    const BeamNode* best = this->getBestNode();
    if (best) {
        qDebug() << "=== Résultat final ===";
        qDebug() << "  Best node score =" << best->score();
        qDebug() << "  Stock final   =" << best->getState().stock.toString();
        qDebug() << "  Time final    =" << best->getState().time;
        qDebug() << "  Dernier process exécuté =" << best->getProcessName();
    } else {
        qDebug() << "Aucun état trouvé.";
    }
}

double BeamSearch::ComputeScore(const BeamState& state, const Scenario& scenario) const
{
    double stock_score = 0.0;
    bool has_target_stock = false;
    bool optimize_time = false;

    for (const QString &target : scenario.optimize_targets) {
        if (target.compare("time", Qt::CaseInsensitive) == 0) {
            optimize_time = true;
        } else {
            int qty = state.stock.value(target);
            stock_score += static_cast<double>(qty) * 1000.0;
            has_target_stock = true;
        }
    }

    if (!has_target_stock && optimize_time) {
        return - _time_penalty * static_cast<double>(state.time);
    }
    if (has_target_stock && optimize_time) {
        double denom = 1.0 + _time_penalty * static_cast<double>(state.time);
        if (denom <= 0.0) denom = 1e-6;
        return stock_score / denom;
    }

    if (has_target_stock && !optimize_time) {
        return stock_score - _time_penalty * static_cast<double>(state.time);
    }

    return 0.0;
}

void BeamSearch::ExpandBeam(QVector<BeamNode>& current_beam, int& next_id)
{
    QVector<BeamNode> new_beam;

    for (const BeamNode& node : current_beam) {
        const BeamState& state = node.getState();

        for (const Process& process : _scenario.processes) {
            if (state.stock.CanApplyProcess(process.needs)) {
                BeamState child_state = state;
                child_state.stock.ApplyProcess(process.needs, process.results);
                child_state.time += process.delay;

                child_state.score = this->ComputeScore(child_state, _scenario);

                QString stockKey = child_state.stock.toStringSorted();
                auto it = _seen_stock_best_time.find(stockKey);
                if (it == _seen_stock_best_time.end()) {
                    _seen_stock_best_time.insert(stockKey, child_state.time);
                }
                else if (child_state.time < it.value()) {
                    it.value() = child_state.time;
                }
                else {
                    continue;
                }

                BeamNode child_node(next_id++, node.getId(), child_state, process.name);
                new_beam.append(child_node);
                _nodes_vector.append(child_node);
            }
        }
    }

    std::sort(new_beam.begin(), new_beam.end(),
              [](const BeamNode& a, const BeamNode& b) {
                  return a.getState().score > b.getState().score;
              });

    if (new_beam.size() > static_cast<size_t>(_beam_size)) {
        new_beam.resize(_beam_size);
    }

    current_beam = std::move(new_beam);
}

const BeamNode* BeamSearch::getBestNode() const
{
    if (_nodes_vector.isEmpty()) return nullptr;

    auto it = std::max_element(
        _nodes_vector.begin(), _nodes_vector.end(), [](const BeamNode& a, const BeamNode& b) 
    {
        return a.score() < b.score();
    });
    return &(*it);
}

QVector<BeamNode> BeamSearch::getsolutionPath() const
{
    QVector<BeamNode> path;
    const BeamNode* best = this->getBestNode();
    if (!best)
        return path;

    QHash<int, BeamNode> map_by_id;
    for (const BeamNode &bn : _nodes_vector) {
        map_by_id.insert(bn.getId(), bn);
    }

    const BeamNode* cur = best;
    while (cur != nullptr && cur->getId() >= 0) {
        path.prepend(*cur);
        int parent_id = cur->getParentId();
        if (parent_id < 0) {
            cur = nullptr;
        } else {
            if (map_by_id.contains(parent_id)) {
                cur = &map_by_id[parent_id];
            } else {
                cur = nullptr;
            }
        }
    }

    return path;
}