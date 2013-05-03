#ifndef __kawaii__process_process_manager__
#define __kawaii__process_process_manager__

#include "kawaii/types.h"
#include "kawaii/scheduler.h"
#include "kawaii/weak_ptr_hash.h"

#include <memory>
#include <list>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Node;
class Process;

typedef unordered_multimap<weak_ptr<Node>,
                           shared_ptr<Process>,
                           WeakPtrHash<Node>,
                           WeakPtrEqual<Node> > ProcessTable;
typedef pair<weak_ptr<Node>, shared_ptr<Process> > ProcessEntry;
typedef pair<ProcessTable::iterator, ProcessTable::iterator> ProcessRange;

class ProcessManager : public UpdateInterface {
public:
    void Attach(shared_ptr<Node> target, shared_ptr<Process> process);
    void Detach(weak_ptr<Node> target);
    void Detach(shared_ptr<Process> process);

    virtual bool Update(const ii_time delta_time);

private:
    ProcessTable process_table_;
};

}

#endif /* defined(__kawaii__process_process_manager__) */
