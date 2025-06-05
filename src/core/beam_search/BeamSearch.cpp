#include "BeamSearch.h"

BeamSearch::BeamSearch(const Scenario& scenario, int beam_size)
    : _scenario(scenario), _beam_size(beam_size)
{
    BeamState initial_state{scenario.initial_stock, 0, 0.0};
    _current_beam.append(BeamNode(0, -1, initial_state, "START"));
    _nodes_vector.append(_current_beam.first());
}

void BeamSearch::RunAlgorithm() {
    _nodes_vector.clear();
    _current_beam.clear();

    BeamState initial_state{_scenario.initial_stock, 0, 0.0};
    initial_state.score = this->ComputeScore(initial_state, _scenario);
    BeamNode root_node(0, -1, initial_state, "START");
    _nodes_vector.append(root_node);
    _current_beam.append(root_node);

    int next_id = 1;

    while (!_current_beam.isEmpty()) {
        this->ExpandBeam(_current_beam, next_id);
    }

    const BeamNode* best = this->getBestNode();
    if (best) {
        qDebug() << "Best node score:" << best->score();
        qDebug() << "Stock:" << best->getState().stock.toString();
        qDebug() << "Time:" << best->getState().time;
        qDebug() << "Process:" << best->getProcessName();
    }
}

double BeamSearch::ComputeScore(const BeamState& state, const Scenario& scenario) const
{
    double score = 0.0;
    bool has_target_stock = false;
    bool optimize_time = false;

    for (const QString &target : scenario.optimize_targets) {
        if (target.trimmed().compare("time", Qt::CaseInsensitive) == 0) {
            optimize_time = true;
        } else {
            score += state.stock.value(target) * 1000;
            has_target_stock = true;
        }
    }

    QMap<QString, int> max_needed;
    for (const Process& process : scenario.processes) {
        for (const QString& target : scenario.optimize_targets) {
            if (process.results.data().contains(target)) {
                for (auto it = process.needs.data().cbegin(); it != process.needs.data().cend(); ++it) {
                    int qty = it.value();
                    max_needed[it.key()] = std::max(max_needed.value(it.key(), 0), qty);
                }
            }
        }
    }

    const QMap<QString, int32_t>& initial_stock = scenario.initial_stock.data();
    for (auto it = state.stock.data().cbegin(); it != state.stock.data().cend(); ++it) {
        QString key = it.key();
        if (max_needed.contains(key) && !scenario.optimize_targets.contains(key) && !initial_stock.contains(key)) {
            int bonus_qty = std::min(it.value(), max_needed[key]);
            score += bonus_qty * 2;
        }
    }

    if (!has_target_stock && optimize_time) {
        return -_time_weight * static_cast<double>(state.time);
    }
    if (has_target_stock && optimize_time) {
        double denom = 1.0 + _time_weight * static_cast<double>(state.time);
        if (denom <= 0.0) denom = 1e-6;
        return score / denom;
    }
    if (has_target_stock && !optimize_time) {
        return score;
    }
    return 0.0;
}

void BeamSearch::ExpandBeam(QVector<BeamNode>& current_beam, int& next_id) {
    QVector<BeamNode> new_beam;

    for (const BeamNode& node : current_beam) {
        const BeamState& state = node.getState();

        for (const Process& process : _scenario.processes) {
            if (state.stock.CanApplyProcess(process.needs)) {
                BeamState new_state = state;
                new_state.stock.ApplyProcess(process.needs, process.results);
                new_state.time += process.delay;
                new_state.score = this->ComputeScore(new_state, _scenario);

                BeamNode new_node(next_id++, node.getId(), new_state, process.name);
                new_beam.append(new_node);
                _nodes_vector.append(new_node);

                qDebug() << "Process applied:" << process.name
                         << "| New Stock:" << new_state.stock.toString()
                         << "| Time:" << new_state.time
                         << "| Score:" << new_state.score;
            }
        }
    }

    std::sort(new_beam.begin(), new_beam.end(), [](const BeamNode& a, const BeamNode& b) {
        return a.getState().score > b.getState().score;
    });

    if (new_beam.size() > _beam_size)
        new_beam.resize(_beam_size);

    current_beam = new_beam;
}

const BeamNode* BeamSearch::getBestNode() const {
    if (_nodes_vector.isEmpty()) return nullptr;
    return &(*std::max_element(_nodes_vector.begin(), _nodes_vector.end(), [](const BeamNode& a, const BeamNode& b) {
        return a.score() < b.score();
    }));
}

QVector<BeamNode> BeamSearch::getSolutionPath() const
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
