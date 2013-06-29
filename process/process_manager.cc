#include "kawaii/process/process_manager.h"

#include "kawaii/process/process.h"

namespace kawaii {

void ProcessManager::Attach(shared_ptr<Process> process) {
    process_list_.push_back(process);
}

bool ProcessManager::Update(const ii_time delta_time) {
    for (ProcessList::iterator i = process_list_.begin(); i != process_list_.end();) {
        shared_ptr<Process> process = (*i);
            
        if (!process->sleeping()) {
            if (!process->running()) {
                process->OnEnter();
                process->set_running(true);
            }
            
            if (!process->Update(delta_time)) {
                process->OnExit();
                process->set_running(false);
                process_list_.erase(i++);
            } else {
                ++i;
            }
        }
    }
    return true;
}

}
