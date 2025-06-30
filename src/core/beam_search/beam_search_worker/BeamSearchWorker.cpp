#include "BeamSearchWorker.h"
#include "TraceWriter.h"

BeamSearchWorker::BeamSearchWorker(const Scenario& scenario, qint64 beam_size, qint64 max_time)
        : _scenario(scenario), _beam_size(beam_size), _max_time(max_time) {}

void BeamSearchWorker::process()
{
    BeamSearch beam_search(_scenario, _beam_size, _max_time);
    beam_search.setScoreCallback([this](double score) {
        emit scoreUpdated(score);
    });
    beam_search.RunAlgorithm();
    
    TraceWriter trace_writer;
    trace_writer.WriteTrace(beam_search.getsolutionPath());
    
    emit finished(beam_search.getNodesVector(), beam_search.getsolutionPath());
}
