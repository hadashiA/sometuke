#ifndef __kawaii__process_scheduler__
#define __kawaii__process_scheduler__

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

    ProcessTimer(shared_ptr<Process> inner_process);
    ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval);
    ProcessTimer(shared_ptr<Process> inner_process, const ii_time interval,
                 const unsigned int repeat, const ii_time delay);

    virtual const HashedString& type() {
        return ProcessTimer::TYPE;
    }

    shared_ptr<Process> inner_process() const {
        return inner_process_;
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
typedef std::list<shared_ptr<ProcessTimer> > TimerList;

class ProcessScheduler {
public:
    void ScheduleFor(shared_ptr<Process> process);
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval);
    void ScheduleFor(shared_ptr<Process> process, const ii_time interval,
                     const unsigned int repeat, const ii_time delay);
    void UnScheduleFor(shared_ptr<Process> process);
    void Update(const ii_time delta_time);

private:
    ProcessList processes_;
};

}

#endif /* defined(__kawaii__process_scheduler__) */
