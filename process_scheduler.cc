#include "process_scheduler.h"

#include "process.h"

#include <climits>

namespace kawaii {

const unsigned int ProcessTimer::REPEAT_FOREVER = UINT_MAX - 1;

ProcessTimer::ProcessTimer(shared_ptr<Process> process)
    : process_(process),
      elapsed_(-1),
      interval_(0),
      delay_(0),
      use_delay_(false),
      repeat_(REPEAT_FOREVER),
      run_forever_(true) {
}

ProcessTimer::ProcessTimer(shared_ptr<Process> process, const ii_time interval)
    : process_(process),
      elapsed_(-1),
      interval_(interval),
      delay_(0),
      use_delay_(false),
      repeat_(REPEAT_FOREVER),
      run_forever_(true) {
}

ProcessTimer::ProcessTimer(shared_ptr<Process> process, const ii_time interval,
                           const unsigned int repeat, const ii_time delay)
    : process_(process),
      elapsed_(-1),
      interval_(interval),
      delay_(delay),
      use_delay_(delay > 0),
      repeat_(repeat),
      run_forever_(repeat == REPEAT_FOREVER) {
}

void ProcessTimer::Update(const ii_time delta_time) {
    // first frame
    if (elapsed_ == -1) {
        elapsed_ = 0;
        num_executed_ = 0;

    // standard timer usage
    } else if (run_forever_ && !use_delay_) {
        elapsed_ += delta_time;
        if (elapsed_ >= interval_) {
            process_->Update(elapsed_);
            elapsed_ = 0;
        }

    // advanced usage
    } else {
        elapsed_ += delta_time;
        if (use_delay_) {
            if (elapsed_ >= delay_) {
                process_->Update(elapsed_);
                elapsed_ -= delay_;
                num_executed_++;
                use_delay_ = false;
            }
        } else {
            if (elapsed_ >= interval_) {
                process_->Update(elapsed_);
                elapsed_ = 0;
                num_executed_++;
            }
        }

        if (num_executed_ > repeat_) {
            process_->Kill();
        }
    }
}

}
