#include "kawaii/process/process_scheduler.h"

#include "kawaii/process/process_timer.h"

#include <climits>

namespace kawaii {

void ProcessScheduler::Attach(shared_ptr<Process> process,
                              const ii_time interval) {
    Process *timer_ptr = new ProcessTimer(process, interval);
    shared_ptr<Process> timer(timer_ptr);
    processes_.push_back(timer);
}
    
void ProcessScheduler::Attach(shared_ptr<Process> process,
                              const ii_time interval,
                              const unsigned int repeat,
                              const ii_time delay) {
    Process *timer_ptr = new ProcessTimer(process, interval, repeat, delay);
    shared_ptr<Process> timer(timer_ptr);
    processes_.push_back(timer);
}

void ProcessScheduler::Update(const ii_time delta_time) {
    for (ProcessList::iterator iter = processes_.begin(); iter != processes_.end();) {
        shared_ptr<Process> p = (*iter);
        ++iter;
            
        if (p->dead()) {
            Detach(p);
        }

        if (!p->paused() && !p->sleeping()) {
            if (!p->Visit(delta_time)) {
                p->Kill();
                Detach(p);
            }
        }
    }

    for (std::list<weak_ptr<UpdateInterface> >::iterator i = update_entries_.begin();
         i != update_entries_.end();) {
        weak_ptr<UpdateInterface> weak_entry = *i;
        if (shared_ptr<UpdateInterface> entry = weak_entry.lock()) {
            entry->Update(delta_time);
            ++i;
        } else {
            i = update_entries_.erase(i);
        }
    }
}

}
