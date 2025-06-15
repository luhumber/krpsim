#include "BeamSearch.h"
#include <algorithm>
#include <QDebug>
#include <QElapsedTimer> // Ajout pour la limite de temps

BeamSearch::BeamSearch(const Scenario& scenario, int beam_size, double max_time)
    : _scenario(scenario)
    , _beam_size(beam_size)
    , _time_penalty(1.0)
    , _max_time(max_time) // Ajout
{
    // On initialise le faisceau avec un seul état “START” en dehors de RunAlgorithm()
    BeamState initial_state{scenario.initial_stock, 0, 0.0};
    initial_state.score = this->ComputeScore(initial_state, _scenario);
    _current_beam.append(BeamNode(0, -1, initial_state, "START"));
    _nodes_vector.append(_current_beam.first());

    // On vide de toute table de déduplication jusqu'au premier appel à RunAlgorithm()
    _seenStockBestTime.clear();
}

void BeamSearch::RunAlgorithm() {
    // Réinitialisation du graphe d'exploration
    _nodes_vector.clear();
    _current_beam.clear();
    _seenStockBestTime.clear();

    // État initial
    BeamState initial_state{_scenario.initial_stock, 0, 0.0};
    initial_state.score = this->ComputeScore(initial_state, _scenario);
    BeamNode root_node(0, -1, initial_state, "START");
    _nodes_vector.append(root_node);
    _current_beam.append(root_node);

    int next_id = 1;

    // Tant que le faisceau courant n'est pas vide, on l'expanse
    while (!_current_beam.isEmpty()) {
        this->ExpandBeam(_current_beam, next_id);
    }

    // On récupère le meilleur noeud parmi tous ceux explorés
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

    // On parcourt la liste des cibles à optimiser
    for (const QString &target : scenario.optimize_targets) {
        if (target.compare("time", Qt::CaseInsensitive) == 0) {
            optimize_time = true;
        } else {
            // Si c'est une ressource “target” (ex. "euro", "tarte_pomme"…),
            // on ajoute au score : 1000 × quantité (on pondère fortement le stock)
            int qty = state.stock.value(target); // Correction ici
            stock_score += static_cast<double>(qty) * 1000.0;
            has_target_stock = true;
        }
    }

    // Cas 1) On optimise explicitement le temps (optimize_time == true)
    if (!has_target_stock && optimize_time) {
        // Tant que l'on n'a pas produit la cible, score = – time
        return - _time_penalty * static_cast<double>(state.time);
    }
    if (has_target_stock && optimize_time) {
        // Lorsque l'état possède déjà une quantité > 0 de la cible,
        // on pondère stock_score par le temps écoulé pour départager :
        double denom = 1.0 + _time_penalty * static_cast<double>(state.time);
        if (denom <= 0.0) denom = 1e-6;
        return stock_score / denom;
    }

    // Cas 2) On optimise uniquement une (ou plusieurs) ressource(s) de stock,
    // et on ne demande pas explicitement “time”
    if (has_target_stock && !optimize_time) {
        // Pour casser les cycles “stock identique mais temps plus long” (ex. séparation↔réunion),
        // on soustrait une petite pénalité proportionnelle au temps.
        // Ainsi deux états avec le même stock mais time différent n'ont pas le même score.
        return stock_score - _time_penalty * static_cast<double>(state.time);
    }

    // Cas 3) Aucune cible ni “time” : on renvoie 0.0
    return 0.0;
}

void BeamSearch::ExpandBeam(QVector<BeamNode>& current_beam, int& next_id) {
    QVector<BeamNode> new_beam;

    for (const BeamNode& node : current_beam) {
        const BeamState& state = node.getState();

        for (const Process& process : _scenario.processes) {
            if (state.stock.CanApplyProcess(process.needs)) {
                BeamState child_state = state;
                child_state.stock.ApplyProcess(process.needs, process.results);
                child_state.time += process.delay;

                // --- AJOUT : on ignore si on dépasse la limite ---
                if (child_state.time > _max_time) {
                    continue;
                }
                // ------------------------------------------------

                child_state.score = this->ComputeScore(child_state, _scenario);

                QString stockKey = child_state.stock.toStringSorted();
                auto it = _seenStockBestTime.find(stockKey);
                if (it == _seenStockBestTime.end()) {
                    _seenStockBestTime.insert(stockKey, child_state.time);
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

                qDebug() << "  → Appliqué :" << process.name
                         << "| New Stock =" << child_state.stock.toString()
                         << "| Time =" << child_state.time
                         << "| Score =" << child_state.score;
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

const BeamNode* BeamSearch::getBestNode() const {
    if (_nodes_vector.isEmpty()) return nullptr;

    auto it = std::max_element(_nodes_vector.begin(), _nodes_vector.end(),
                               [](const BeamNode& a, const BeamNode& b) {
                                   return a.score() < b.score();
                               });
    return &(*it);
}

QVector<BeamNode> BeamSearch::getSolutionPath() const
{
    QVector<BeamNode> path;
    const BeamNode* best = this->getBestNode();
    if (!best)
        return path;

    // On construit une table id → BeamNode pour la remontée au parent
    QHash<int, BeamNode> map_by_id;
    for (const BeamNode &bn : _nodes_vector) {
        map_by_id.insert(bn.getId(), bn);
    }

    // On remonte du meilleur jusqu'à la racine (id parent < 0)
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
