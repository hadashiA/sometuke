#include "sequence.h"

namespace kawaii {

const HashedString Sequence::TYPE("process:sequence");

void Sequence::Update(const ii_time delta_time) {
    shared_ptr<Process> process = processes_.front();
    if (!process->dead()) {
        process->Update(delta_time);
    }

    if (process->dead()) {
        processes_.pop_front();
        if (processes_.empty()) {
            Kill();
            return;
        }
        process = processes_.front();
        process->OnEnter();
    }
}

}
