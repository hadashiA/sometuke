#ifndef __kawaii__process_process_scheduler__
#define __kawaii__process_process_scheduler__

#include "types.h"
#include "process.h"

#include <memory>
#include <list>

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

    virtual void OnEnter() {
        return inner_process_->OnEnter();
    }

    virtual void Update(const ii_time delta_time);

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

typedef std::list<shared_ptr<Process> > ProcessList;

class Node;

class ProcessScheduler {
public:
    void ScheduleFor(shared_ptr<Process> process);
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval);
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval,
                     const unsigned int repeat, const ii_time delay);
    void ScheduleFor(weak_ptr<Node> node);

    void UnScheduleFor(shared_ptr<Process> process);
    void UnScheduleFor(weak_ptr<Node> node);

    void Update(const ii_time delta_time);

private:
    ProcessList processes_;
    std::list<weak_ptr<Node> > nodes_;
};

}

#endif /* defined(__kawaii__process_process_scheduler__) */
