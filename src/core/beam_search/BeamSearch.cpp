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
    BeamNode rootNode(0, -1, initial_state, "START");
    _nodes_vector.append(rootNode);
    _current_beam.append(rootNode);

    int nextId = 1;

    while (!_current_beam.isEmpty()) {
        this->ExpandBeam(_current_beam, nextId);
    }

    const BeamNode* best = this->getBestNode();
    if (best) {
        qDebug() << "Best node score:" << best->score();
        qDebug() << "Stock:" << best->getState().stock.toString();
        qDebug() << "Time:" << best->getState().time;
        qDebug() << "Process:" << best->getProcessName();
    }
}

// double BeamSearch::ComputeScore(const BeamState& state, const Scenario& scenario) const {
//     double score = 0.0;
//     for (const auto& target : scenario.optimize_targets) {
//         score += state.stock.value(target);
//     }
//     return score;
// }

double BeamSearch::ComputeScore(const BeamState& state, const Scenario& scenario) const
{
    // 1) Calculer la somme S des quantités de tous les stocks listés dans optimize_targets,
    //    sauf “time” qui est traitée plus bas.
    double S = 0.0;
    bool sawAnyStock    = false;
    bool sawTimeKeyword = false;

    for (const QString &cible : scenario.optimize_targets) {
        if (cible.trimmed().compare("time", Qt::CaseInsensitive) == 0) {
            sawTimeKeyword = true;
        } else {
            // On suppose que cible est le nom d’un stock : ajout de sa quantité
            S += static_cast<double>(state.stock.value(cible));
            sawAnyStock = true;
        }
    }

    // 2) Cas où on ne demande que “time” (aucun stock à maximiser)
    if (!sawAnyStock && sawTimeKeyword) {
        // On retourne un score négatif proportionnel au temps (pour minimiser time)
        return -_timeWeight * static_cast<double>(state.time);
    }

    // 3) Cas “classique” où on veut maximiser S et (éventuellement) minimiser time
    if (sawAnyStock && sawTimeKeyword) {
        // Formule “ratio” : (stock total) / (1 + alpha * time)
        double denom = 1.0 + _timeWeight * static_cast<double>(state.time);
        if (denom <= 0.0) denom = 1e-6; // protection, même si time≥0
        return S / denom;
    }

    // 4) Cas où on ne s’intéresse qu’aux stocks (pas de “time” dans optimize_targets)
    if (sawAnyStock && !sawTimeKeyword) {
        // On maximise simplement la quantité totale S (le temps n’entre pas dans le score)
        return S;
    }

    // 5) Si optimize_targets était vide ou invalide, on retourne un score par défaut (=0)
    return 0.0;
}

void BeamSearch::ExpandBeam(QVector<BeamNode>& current_beam, int& nextId) {
    QVector<BeamNode> new_beam;

    for (const BeamNode& node : current_beam) {
        const BeamState& state = node.getState();

        for (const Process& process : _scenario.processes) {
            if (state.stock.CanApplyProcess(process.needs)) {
                BeamState newState = state;
                newState.stock.ApplyProcess(process.needs, process.results);
                newState.time += process.delay;
                newState.score = this->ComputeScore(newState, _scenario);

                BeamNode newNode(nextId++, node.getId(), newState, process.name);
                new_beam.append(newNode);
                _nodes_vector.append(newNode);

                qDebug() << "Process applied:" << process.name
                         << "| New Stock:" << newState.stock.toString()
                         << "| Time:" << newState.time
                         << "| Score:" << newState.score;
            } else {

                /*qDebug() << "Process not applicable:" << process.name
                         << "| Stock:" << state.stock.toString()
                         << "| Needs:" << process.needs.toString();*/
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

    // Créer un map temporaire id -> BeamNode pour pouvoir 
    // rechercher rapidement un BeamNode à partir de son ID
    QHash<int, BeamNode> mapById;
    for (const BeamNode &bn : _nodes_vector) {
        mapById.insert(bn.getId(), bn);
    }

    // Remonter depuis le meilleur nœud jusqu’à la racine (parentId == -1)
    const BeamNode* cur = best;
    while (cur != nullptr && cur->getId() >= 0) {
        path.prepend(*cur);  // on ajoute en tête pour avoir racine → feuille
        int pid = cur->getParentId();
        if (pid < 0) {
            cur = nullptr; 
        } else {
            // Chercher l’utilisateur par son ID dans mapById
            if (mapById.contains(pid)) {
                cur = &mapById[pid];
            } else {
                // en cas de donnée incohérente, on s’arrête
                cur = nullptr;
            }
        }
    }

    return path;
}
