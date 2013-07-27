#include "sometuke/scheduler.h"

#include "sometuke/director.h"

#include <climits>
#include <cmath>
#include <sys/time.h>

namespace sometuke {
const unsigned int Timer::REPEAT_FOREVER(UINT_MAX - 1);

bool Timer::Tick(const s2_time delta_time) {
    // standard timer usage
    if (run_forever_ && !use_delay_) {
        elapsed_ += delta_time;
        if (elapsed_ >= interval_) {
            bool result = Update(elapsed_);
            elapsed_ = 0;
            return result;
        }
        
        // advanced usage
    } else {
        elapsed_ += delta_time;
        if (use_delay_) {
            if (elapsed_ >= delay_) {
                elapsed_ -= delay_;
                num_executed_++;
                use_delay_ = false;
                return Update(elapsed_);
            }
        } else {
            if (elapsed_ >= interval_) {
                elapsed_ = 0;
                num_executed_++;
                return Update(elapsed_);
            }
        }
        
        if (num_executed_ >= repeat_) {
            return false;
        }
    }
    return true;
}

void Timer::Schedule() {
    Director::Instance().scheduler().Schedule(shared_from_this());
}

void Timer::UnSchedule() {
    Director::Instance().scheduler().Unschedule(shared_from_this());
}

double Scheduler::Now() {
    timeval tv;
    gettimeofday(&tv, NULL);
    
    double msec = floor(tv.tv_usec * 0.001) * 0.001;
    return ((double)(tv.tv_sec) + msec);
}

void Scheduler::Unschedule(const weak_ptr<Timer>& timer) {
    for (TimerList::iterator i = timers_.begin(); i != timers_.end();) {
        if (timer.lock() == i->lock()) {
            timers_.erase(i++);
        } else {
            ++i;            
        }
    }
}

void Scheduler::Update(const s2_time delta_time) {
    for (TimerList::iterator i = timers_.begin(); i != timers_.end();) {
        if (i->expired()) {
            timers_.erase(i++);
        } else {
            shared_ptr<Timer> timer = i->lock();
            if (!timer->paused()) {
                timer->Tick(delta_time);
            }
            ++i;
        }
    }
}

}
