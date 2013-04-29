#ifndef __kawaii__process_process_scheduler__
#define __kawaii__process_process_scheduler__

#include "kawaii/types.h"

#include <memory>
#include <list>
#include <cassert>

namespace kawaii {
using namespace std;

class Process;
class Node;

typedef list<shared_ptr<Process> > ProcessList;

class UpdateInterface {
public:
    virtual ~UpdateInterface() {}
    virtual void Update(const ii_time delta_time) = 0;
};

class ProcessScheduler {
public:
    void Attach(shared_ptr<Process> process) {
        processes_.push_back(process);
    }

    void Attach(shared_ptr<Process> process, const ii_time interval);
    void Attach(shared_ptr<Process> process, const ii_time interval,
                const unsigned int repeat, const ii_time delay);

    void Attach(weak_ptr<UpdateInterface> update_entry) {
        if (!update_entry.expired()) {
            update_entries_.push_back(update_entry);
        }
    }

    void Detach(shared_ptr<Process> process) {
        processes_.remove(process);
    }

    void Detach(weak_ptr<UpdateInterface> update_entry) {
        assert(false);              // no implemented yet
        // nodes_.remove(node);
    }

    void Update(const ii_time delta_time);

private:
    ProcessList processes_;
    list<weak_ptr<UpdateInterface> > update_entries_;
};

}

#endif /* defined(__kawaii__process_process_scheduler__) */
