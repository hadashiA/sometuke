#include "kawaii/process/process_scheduler.h"

#include "kawaii/process/process_timer.h"

#include <climits>

namespace kawaii {

void ProcessScheduler::Attach(shared_ptr<Process> process) {
    processes_.push_back(process);
}

void ProcessScheduler::Attach(shared_ptr<Process> process,
                              const ii_time interval) {
    Process *timer_ptr = new ProcessTimer(process, interval);
    shared_ptr<Process> timer(timer_ptr);
    Attach(timer);
}
    
void ProcessScheduler::Attach(shared_ptr<Process> process,
                              const ii_time interval,
                              const unsigned int repeat,
                              const ii_time delay) {
    Process *timer_ptr = new ProcessTimer(process, interval, repeat, delay);
    shared_ptr<Process> timer(timer_ptr);
    Attach(timer);
}

void ProcessScheduler::Detach(shared_ptr<Process> process) {
    processes_.remove(process);
}

void ProcessScheduler::Update(const ii_time delta_time) {
    for (ProcessList::iterator iter = processes_.begin(); iter != processes_.end();) {
        shared_ptr<Process> p = (*iter);
        ++iter;
            
        if (!p->Visit(delta_time)) {
            p->End();
            Detach(p);
        }
    }
}

}
