#ifndef __sometuke__process_process_timer__
#define __sometuke__process_process_timer__

#include "sometuke/process/process.h"

namespace sometuke {

class ProcessTimer : public Process {
public:
    static const HashedString TYPE;
    static const unsigned int REPEAT_FOREVER;

    ProcessTimer(shared_ptr<Process> inner_process)
        : inner_process_(inner_process),
          elapsed_(0),
          num_executed_(0),
          interval_(0),
          delay_(0),
          use_delay_(false),
          repeat_(REPEAT_FOREVER),
          run_forever_(true) {
    }

    ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval)
        : inner_process_(inner_process),
          elapsed_(0),
          num_executed_(0),
          interval_(interval),
          delay_(0),
          use_delay_(false),
          repeat_(REPEAT_FOREVER),
          run_forever_(true) {
    }

    ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval,
                 const unsigned int repeat, const ii_time delay)
        : inner_process_(inner_process),
          elapsed_(0),
          num_executed_(0),
          interval_(interval),
          delay_(delay),
          use_delay_(delay > 0),
          repeat_(repeat),
          run_forever_(repeat == REPEAT_FOREVER) {
    }

    virtual const HashedString& type() const {
        return ProcessTimer::TYPE;
    }

    shared_ptr<Process> inner_process() const {
        return inner_process_;
    }

    virtual void OnEnter() {
        inner_process_->Enter();
    }

    virtual void OnExit() {
        inner_process_->Exit();
    }

    virtual bool Update(const ii_time delta_time);

private:    
    shared_ptr<Process> inner_process_;
    ii_time elapsed_;
    ii_time interval_;
    ii_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
};

}

#endif /* defined(__Hitasura__process_process_timer__) */
