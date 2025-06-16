#include "BeamSearchWorker.h"

BeamSearchWorker::BeamSearchWorker(const Scenario& scenario, int beam_size)
        : _scenario(scenario), _beam_size(beam_size) {}

void BeamSearchWorker::process() {
    BeamSearch beam_search(_scenario, _beam_size);
    beam_search.setScoreCallback([this](double score) {
        emit scoreUpdated(score);
    });
    beam_search.RunAlgorithm();
    emit finished(beam_search.getNodesVector(), beam_search.getSolutionPath());
}
