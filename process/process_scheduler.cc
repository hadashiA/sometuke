#include "process_scheduler.h"

#include "node.h"
#include "process_timer.h"

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

    for (std::list<weak_ptr<Node> >::iterator iter = nodes_.begin(); iter != nodes_.end();) {
        weak_ptr<Node> node_ref = *iter;
        if (shared_ptr<Node> node = node_ref.lock()) {
            node->Update(delta_time);
            ++iter;
        } else {
            iter = nodes_.erase(iter);
        }
    }
}

}
