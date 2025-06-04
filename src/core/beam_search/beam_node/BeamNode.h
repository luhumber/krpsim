#pragma once
#include <QString>
#include "BeamState.h"

class BeamNode {
public:
    BeamNode() = default;
    BeamNode(int id, int parentId, const BeamState& state, const QString& processName);

    int getId() const { return _id; }
    int getParentId() const { return _parent_id; }
    BeamState getState() const { return _state; }
    QString getProcessName() const { return _process_name; }
    double score() const { return _state.score; }

private:
    int         _id;
    int         _parent_id;
    BeamState   _state;
    QString     _process_name;
};