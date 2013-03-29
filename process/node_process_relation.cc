#include "node_process_relation.h"

#include "application.h"
#include "move_to.h"
#include "move_by.h"

namespace kawaii {

void NodeProcessRelation::Run() {
    Application::Instance().director().scheduler().Attach(sequence_);
}

NodeProcessRelation NodeProcessRelation::MoveTo(const ii_time duration, const vec3& to) {
    shared_ptr<Process> move_to(new class MoveTo(target_, duration, to));
    static_pointer_cast<Sequence>(sequence_)->Push(move_to);
    return *this;
}

NodeProcessRelation NodeProcessRelation::MoveBy(const ii_time duration, const vec3& delta) {
    shared_ptr<Process> move_by(new class MoveBy(target_, duration, delta));
    static_pointer_cast<Sequence>(sequence_)->Push(move_by);
    return *this;
}

}
