#include "process_manager.h"

#include "process.h"

namespace kawaii {

void ProcessManager::Attach(const shared_ptr<Process>& process) {
    processes_.push_back(process);
}

void ProcessManager::Detach(const shared_ptr<Process>& process) {
    processes_.remove(process);
}

bool ProcessManager::IsProcessActive(const HashedString& process_type) {
    for (ProcessList::iterator i = processes_.begin(); i != processes_.end(); ++i) {
        shared_ptr<Process> process = *i;
        if (process->type() == process_type && (!process->dead() || process->next())) {
            return true;
        }
    }
    return false;
}

}
