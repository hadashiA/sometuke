#include "process_scheduler.h"

#include "node.h"

#include <climits>

namespace kawaii {

// ProcessTimer

const HashedString ProcessTimer::TYPE("process:timer");
const unsigned int ProcessTimer::REPEAT_FOREVER(UINT_MAX - 1);

bool ProcessTimer::Update(const ii_time delta_time) {
    // standard timer usage
    if (run_forever_ && !use_delay_) {
        elapsed_ += delta_time;
        if (elapsed_ >= interval_) {
            elapsed_ = 0;
            return inner_process_->Visit(elapsed_);
        }
        
    // advanced usage
    } else {
        elapsed_ += delta_time;
        if (use_delay_) {
            if (elapsed_ >= delay_) {
                elapsed_ -= delay_;
                num_executed_++;
                use_delay_ = false;
                return inner_process_->Visit(elapsed_);
            }
        } else {
            if (elapsed_ >= interval_) {
                elapsed_ = 0;
                num_executed_++;
                return inner_process_->Visit(elapsed_);
            }
        }
        
        if (num_executed_ >= repeat_) {
            return false;
        }
    }
    return true;
}

// ProcessScheduler

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
