#include "kawaii/process/process_manager.h"

#include "kawaii/process/process.h"

namespace kawaii {

void ProcessManager::Attach(shared_ptr<Node> target, shared_ptr<Process> process) {
    process->StartWithTarget(target);
    process_table_.insert(make_pair(target, process));
}

void ProcessManager::Detach(weak_ptr<Node> target) {
    ProcessRange range = process_table_.equal_range(target);
    for (ProcessTable::iterator i = range.first; i != range.second; ++i) {
        i->second->End();
    }
    process_table_.erase(range.first, range.second);
}

void ProcessManager::Detach(shared_ptr<Process> process) {
    for (ProcessTable::iterator i = process_table_.begin(); i != process_table_.end();) {
        if (process == i->second) {
            process->End();
            process_table_.erase(i++);
        } else {
            ++i;
        }
    }
}

bool ProcessManager::Update(const ii_time delta_time) {
    for (ProcessTable::iterator i = process_table_.begin(); i != process_table_.end();) {
        weak_ptr<Node> target_weak  = i->first;
        shared_ptr<Process> process = i->second;
            
        if (target_weak.expired() || !process->Update(delta_time)) {
            process->End();
            process_table_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

}
