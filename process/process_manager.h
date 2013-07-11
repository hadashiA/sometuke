#ifndef __skidarake__process_process_manager__
#define __skidarake__process_process_manager__

#include "skidarake/types.h"
#include "skidarake/scheduler.h"
#include "skidarake/weak_ptr_hash.h"

#include <memory>
#include <list>
#include <unordered_map>

namespace skidarake {
using namespace std;

class Process;

typedef list<shared_ptr<Process> > ProcessList;

class ProcessManager : public UpdateInterface {
public:
    shared_ptr<Process> Attach(const shared_ptr<Process>& process);
    void Dettach(const shared_ptr<Process>& process);
    virtual bool Update(const ii_time delta_time);

private:
    ProcessList process_list_;
};

}

#endif /* defined(__skidarake__process_process_manager__) */
