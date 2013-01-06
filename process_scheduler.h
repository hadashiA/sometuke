#ifndef __kawaii__process_scheduler__
#define __kawaii__process_scheduler__

#include "types.h"

#include <memory>
#include <list>

namespace kawaii {
using namespace std;

class Process;

class ProcessTimer {
public:
    static const unsigned int REPEAT_FOREVER;

    ProcessTimer(shared_ptr<Process> process);
    ProcessTimer(shared_ptr<Process> process, const ii_time interval);
    ProcessTimer(shared_ptr<Process> process, const ii_time interval,
                 const unsigned int repeat, const ii_time delay);

    shared_ptr<Process> process() const {
        return process_;
    }

    void Update(const ii_time delta_time);

private:    
    shared_ptr<Process> process_;
    ii_time elapsed_;
    ii_time interval_;
    ii_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
};

typedef std::list<shared_ptr<ProcessTimer> > TimerList;

class ProcessScheduler {
public:
    void Update(const ii_time delta_time);
    void ScheduleFor(shared_ptr<Process> process);
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval);
    void UnScheduleFor(shared_ptr<Process> process);

private:
    TimerList timers_;
};

}

#endif /* defined(__kawaii__process_scheduler__) */
