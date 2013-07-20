#include "sometuke/process/sequence.h"

#include "sometuke/logger.h"

namespace sometuke {

const HashedString Sequence::TYPE("process:sequence");

bool Sequence::Update(const ii_time delta_time) {
    shared_ptr<Process> process = processes_[current_];
    bool continued = (process && process->Update(delta_time));

    if (continued) {
        return true;
    } else {
        process->Exit();
        if (++current_ >= processes_.size()) {
            return false;
        } else {
            shared_ptr<Process> next = processes_[current_];
            next->Enter();
            return true;
        }
    }
}

}
