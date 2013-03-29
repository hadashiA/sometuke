#ifndef __kawaii__process_process_scheduler__
#define __kawaii__process_process_scheduler__

#include "types.h"

#include <memory>
#include <list>
#include <cassert>

namespace kawaii {
using namespace std;

class Process;
class Node;

typedef list<shared_ptr<Process> > ProcessList;

class ProcessScheduler {
public:
    void Attach(shared_ptr<Process> process) {
        processes_.push_back(process);
    }

    void Attach(shared_ptr<Process> process, const ii_time interval);
    void Attach(shared_ptr<Process> process, const ii_time interval,
                const unsigned int repeat, const ii_time delay);

    void Attach(weak_ptr<Node> node) {
        if (!node.expired()) {
            nodes_.push_back(node);
        }
    }

    void Detach(shared_ptr<Process> process) {
        processes_.remove(process);
    }

    void Detach(weak_ptr<Node> node) {
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
