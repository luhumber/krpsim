#include "BeamNode.h"

BeamNode::BeamNode(int id, int parent_id, const BeamState& state, const QString& processName)
    : _id(id), _parent_id(parent_id), _state(state), _process_name(processName) { }
