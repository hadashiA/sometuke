#ifndef __kawaii__process_process_scheduler__
#define __kawaii__process_process_scheduler__

#include "types.h"
#include "process.h"

#include <memory>
#include <list>
#include <cassert>

namespace kawaii {
using namespace std;

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

    virtual const HashedString& type() {
        return ProcessTimer::TYPE;
    }

    shared_ptr<Process> inner_process() const {
        return inner_process_;
    }

private:    
    virtual void OnEnter() {
        inner_process_->OnEnter();
    }

    virtual void OnExit() {
        inner_process_->OnExit();
    }

    virtual bool Update(const ii_time delta_time);

    shared_ptr<Process> inner_process_;
    ii_time elapsed_;
    ii_time interval_;
    ii_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
};

typedef list<shared_ptr<Process> > ProcessList;

class Node;

class ProcessScheduler {
public:
    void ScheduleFor(shared_ptr<Process> process) {
        processes_.push_back(process);
    }

    void ScheduleFor(shared_ptr<Process> process, const ii_time interval) {
        Process *timer_ptr = new ProcessTimer(process, interval);
        shared_ptr<Process> timer(timer_ptr);
        processes_.push_back(timer);
    }
    
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval,
                     const unsigned int repeat, const ii_time delay) {
        Process *timer_ptr = new ProcessTimer(process, interval, repeat, delay);
        shared_ptr<Process> timer(timer_ptr);
        processes_.push_back(timer);
    }

    void ScheduleFor(weak_ptr<Node> node) {
        if (!node.expired()) {
            nodes_.push_back(node);
        }
    }

    void UnScheduleFor(shared_ptr<Process> process) {
        processes_.remove(process);
    }

    void UnScheduleFor(weak_ptr<Node> node) {
        assert(false);              // no implemented yet
        // nodes_.remove(node);
    }

    void Update(const ii_time delta_time);

private:
    ProcessList processes_;
    list<weak_ptr<Node> > nodes_;
};

}

#endif /* defined(__kawaii__process_process_scheduler__) */
