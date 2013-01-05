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
    ProcessTimer(shared_ptr<Process> process);
    ProcessTimer(shared_ptr<Process> process, ii_time interval);
    ProcessTimer(shared_ptr<Process> process, ii_time interval, int repeat, ii_time delay);

private:    
    shared_ptr<Process> process_;
    ii_time elapsed_;
    bool run_forever_;
    bool use_delay_;
    int num_executed_;
    int repeat;
    ii_time delay;
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
