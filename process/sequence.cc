#include "kawaii/process/sequence.h"

#include "kawaii/logger.h"

namespace kawaii {

const HashedString Sequence::TYPE("process:sequence");

bool Sequence::Step(const ii_time delta_time) {
    shared_ptr<Process> process = processes_[current_];
    bool continued = (process && process->Update(delta_time));

    if (continued) {
        return true;
    } else {
        process->End();
        if (++current_ >= processes_.size()) {
            return false;
        } else {
            shared_ptr<Process> next = processes_[current_];
            next->StartWithTarget(target_);
            return true;
        }
    }
}

}
