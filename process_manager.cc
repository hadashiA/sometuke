#include "process_manager.h"

#include "process.h"

namespace kawaii {

ProcessManager::~ProcessManager() {
    for (ProcessList::iterator i = processes_.begin(); i != processes_.end(); i++) {
        Detach(*i);
    }
}

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

bool ProcessManager::HasProcesses() const {
    return !processes_.empty();
}

void ProcessManager::UpdateProcesses(const float deltaTime) {
    for (ProcessList::iterator i = processes_.begin(); i != processes_.end(); i++) {
        shared_ptr<Process> p = *i;
        if (p->dead()) {
            shared_ptr<Process> next = p->next();
            if (next) {
                p->set_next(NULL);
                Attach(next);
            }
            Detach(p);
        } else if (p->is_active() && !p->paused()) {
            p->Update(deltaTime);
        }
    }
}

}
