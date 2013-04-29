#include "kawaii/process/process_timer.h"

namespace kawaii {

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

}
