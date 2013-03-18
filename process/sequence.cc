#include "sequence.h"

#include "logger.h"

namespace kawaii {

const HashedString Sequence::TYPE("process:sequence");

bool Sequence::Update(const ii_time delta_time) {
    shared_ptr<Process> process = processes_[current_];
    bool continued = (process && process->Visit(delta_time));

    if (continued) {
        return true;
    } else {
        process->OnExit();
        if (++current_ >= processes_.size()) {
            return false;
        } else {
            shared_ptr<Process> next = processes_[current_];
            next->OnEnter();
            return true;
        }
    }
}

}