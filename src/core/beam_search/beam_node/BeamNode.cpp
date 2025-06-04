#include "BeamNode.h"

BeamNode::BeamNode(int id, int parentId, const BeamState& state, const QString& processName)
    : _id(id), _parent_id(parentId), _state(state), _process_name(processName) {
    // Constructor implementation can be extended if needed
}
