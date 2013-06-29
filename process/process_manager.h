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

class Process;

typedef list<shared_ptr<Process> > ProcessList;

class ProcessManager : public UpdateInterface {
public:
    void Attach(shared_ptr<Process> process);
    virtual bool Update(const ii_time delta_time);

private:
    ProcessList process_list_;
};

}

#endif /* defined(__kawaii__process_process_manager__) */
