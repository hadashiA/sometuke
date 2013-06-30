#include "kawaii/process/process_manager.h"

#include "kawaii/process/process.h"
#include "kawaii/process/sequence.h"

namespace kawaii {

void ProcessManager::Attach(const shared_ptr<Process>& process) {
    process_list_.push_back(process);
    return process;
}

void Dettach(const shared_ptr<Process>& process) {
    for (ProcessList::iterator i = process_list_.begin(); i != process_list_.end();) {
        if ((*i) == process) {
            process_list_.erase(i++);
            break;
        } else {
            ++i;
        }
    }
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
