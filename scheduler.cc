#include "sometuke/scheduler.h"

#include "sometuke/director.h"

#include <climits>

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

}
