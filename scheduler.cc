#include "scheduler.h"

namespace kawaii {

const unsigned int Timer::REPEAT_FOREVER(UINT_MAX - 1);

bool Timer::Update(const ii_time delta_time) {
    // standard timer usage
    if (run_forever_ && !use_delay_) {
        elapsed_ += delta_time;
        if (elapsed_ >= interval_) {
            elapsed_ = 0;
            return delegate_->Update(elapsed_);
        }
        
    // advanced usage
    } else {
        elapsed_ += delta_time;
        if (use_delay_) {
            if (elapsed_ >= delay_) {
                elapsed_ -= delay_;
                num_executed_++;
                use_delay_ = false;
                return delegate_->Update(elapsed_);
            }
        } else {
            if (elapsed_ >= interval_) {
                elapsed_ = 0;
                num_executed_++;
                return delegate_->Update(elapsed_);
            }
        }
        
        if (num_executed_ >= repeat_) {
            return false;
        }
    }
    return true;
}


void Scheduler::Schedule(shared_ptr<UpdateInterface> delegate) {
    update_list_.push_back(delegate);
}

void Scheduler::Schedule(shared_ptr<UpdateInterface> delegate, const ii_time interval) {
    shared_ptr<UpdateInterface> timer = make_shared<Timer>(delegate, interval);
    Attach(timer);
}
    
void Scheduler::Schedule(shared_ptr<UpdateInterface> delegate,
                         const ii_time interval,
                         const unsigned int repeat,
                         const ii_time delay) {
    shared_ptr<UpdateInterface> timer = make_shared<Timer>(delegate, interval, repeat, delay);
    Attach(timer);
}

void Scheduler::Unschedule(shared_ptr<UpdateInterface> delegate) {
    update_list_.remove(delegate);
}

void Scheduler::Update(const ii_time delta_time) {
    for (UpdateList::iterator i = update_list_.begin(); i != update_list_.end();) {
        shared_ptr<UpdateInterface> p = (*i);
        ++iter;
            
        if (!p->Update(delta_time)) {
            Detach(p);
        }
    }
}

}
