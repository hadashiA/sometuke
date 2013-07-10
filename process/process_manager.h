#ifndef __skidarak__process_process_manager__
#define __skidarak__process_process_manager__

#include "skidarak/types.h"
#include "skidarak/scheduler.h"
#include "skidarak/weak_ptr_hash.h"

#include <memory>
#include <list>
#include <unordered_map>

namespace skidarak {
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

#endif /* defined(__skidarak__process_process_manager__) */
