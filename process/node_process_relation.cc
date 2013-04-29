#include "kawaii/process/node_process_relation.h"

#include "kawaii/application.h"
#include "kawaii/process/move_to.h"
#include "kawaii/process/move_by.h"
#include "kawaii/process/repeat.h"

namespace kawaii {

void NodeProcessRelation::Run() {
    Application::Instance().director().scheduler().Attach(sequence_);
}

void NodeProcessRelation::Repeat(int n) {
    shared_ptr<Process> repeat(new class Repeat(sequence_, n));
    Application::Instance().director().scheduler().Attach(repeat);
}

NodeProcessRelation NodeProcessRelation::MoveTo(const ii_time duration,
                                                const vec3& to) {
    shared_ptr<Process> move_to(new class MoveTo(target_, duration, to));
    sequence_->Push(move_to);
    return *this;
}

NodeProcessRelation NodeProcessRelation::MoveBy(const ii_time duration,
                                                const vec3& delta) {
    shared_ptr<Process> move_by(new class MoveBy(target_, duration, delta));
    sequence_->Push(move_by);
    return *this;
}

}
