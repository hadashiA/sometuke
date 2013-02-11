#include "process_scheduler.h"

#include "node.h"

#include <climits>

namespace kawaii {

const HashedString ProcessTimer::TYPE("timer process");
const unsigned int ProcessTimer::REPEAT_FOREVER(UINT_MAX - 1);

// ProcessTimer

ProcessTimer::ProcessTimer(shared_ptr<Process> inner_process)
    : inner_process_(inner_process),
      elapsed_(-1),
      interval_(0),
      delay_(0),
      use_delay_(false),
      repeat_(REPEAT_FOREVER),
      run_forever_(true) {
}

ProcessTimer::ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval)
    : inner_process_(inner_process),
      elapsed_(-1),
      interval_(interval),
      delay_(0),
      use_delay_(false),
      repeat_(REPEAT_FOREVER),
      run_forever_(true) {
}

ProcessTimer::ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval,
                           const unsigned int repeat, const ii_time delay)
    : inner_process_(inner_process),
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

    } else {
        // standard timer usage
        if (run_forever_ && !use_delay_) {
            elapsed_ += delta_time;
            if (elapsed_ >= interval_) {
                inner_process_->Update(elapsed_);
                elapsed_ = 0;
            }
            
        // advanced usage
        } else {
            elapsed_ += delta_time;
            if (use_delay_) {
                if (elapsed_ >= delay_) {
                    inner_process_->Update(elapsed_);
                    elapsed_ -= delay_;
                    num_executed_++;
                    use_delay_ = false;
                }
            } else {
                if (elapsed_ >= interval_) {
                    inner_process_->Update(elapsed_);
                    elapsed_ = 0;
                    num_executed_++;
                }
            }
            
            if (num_executed_ >= repeat_) {
                inner_process_->Kill();
            }
        }

        if (inner_process_->dead()) {
            shared_ptr<Process> next = inner_process_->next();
            if (next) {
                inner_process_->set_next(NULL);
                inner_process_ = next;
            } else {
                Kill();
            }
        }
    }
}

// ProcessScheduler

void ProcessScheduler::ScheduleFor(shared_ptr<Process> process) {
    processes_.push_back(process);
}

void ProcessScheduler::ScheduleFor(shared_ptr<Process> process, const ii_time interval) {
    Process *timer_ptr = new ProcessTimer(process, interval);
    shared_ptr<Process> timer(timer_ptr);
    processes_.push_back(timer);
}
    
void ProcessScheduler::ScheduleFor(shared_ptr<Process> process, const ii_time interval,
                                   const unsigned int repeat, const ii_time delay) {
    Process *timer_ptr = new ProcessTimer(process, interval, repeat, delay);
    shared_ptr<Process> timer(timer_ptr);
    processes_.push_back(timer);
}

void ProcessScheduler::ScheduleFor(Node *node) {
    nodes_.push_back(node);
}

void ProcessScheduler::UnScheduleFor(shared_ptr<Process> process) {
    processes_.remove(process);
}

void ProcessScheduler::UnScheduleFor(Node *node) {
    nodes_.remove(node);
}

void ProcessScheduler::Update(const ii_time delta_time) {
    for (ProcessList::iterator iter = processes_.begin(); iter != processes_.end();) {
        shared_ptr<Process> p = (*iter);
        ++iter;
            
        if (p->dead()) {
            shared_ptr<Process> next = p->next();
            if (next) {
                p->set_next(NULL);
                ScheduleFor(next);
            }
            UnScheduleFor(p);
        } else if (p->is_active() && !p->paused()) {
            p->Update(delta_time);
        }
    }

    for (std::list<Node *>::iterator iter = nodes_.begin(); iter != nodes_.end(); ++iter) {
        Node *node = (*iter);
        node->Update(delta_time);
    }
}

}
