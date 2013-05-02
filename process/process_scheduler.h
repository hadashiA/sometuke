#ifndef __kawaii__process_process_scheduler__
#define __kawaii__process_process_scheduler__

#include "kawaii/types.h"

#include <memory>
#include <list>
#include <map>
#include <cassert>

namespace kawaii {
using namespace std;

class Process;
class UpdateInterface;

typedef list<shared_ptr<Process> > ProcessList;

class ProcessScheduler {
public:
    void Attach(shared_ptr<Process> process);
    void Attach(shared_ptr<Process> process, const ii_time interval);
    void Attach(shared_ptr<Process> process, const ii_time interval,
                const unsigned int repeat, const ii_time delay);

    void Detach(shared_ptr<Process> process);

    void ScheduleUpdateFor(weak_ptr<UpdateInterface> update_entry);
    void UnScheduleUpdateFor(weak_ptr<UpdateInterface> update_entry);

    void Update(const ii_time delta_time);

private:
    ProcessList processes_;
};

}

#endif /* defined(__kawaii__process_process_scheduler__) */
