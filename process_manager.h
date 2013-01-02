#ifndef __kawaii__process_manager__
#define __kawaii__process_manager__

#include "hashed_string.h"

#include <memory>
#include <list>

namespace kawaii {
using namespace std;

class Process;

typedef std::list<shared_ptr<Process> > ProcessList;

class ProcessManager {
public:
    ~ProcessManager();

    void Attach(const shared_ptr<Process>& process);
    bool HasProcesses() const;
    bool IsProcessActive(const HashedString& process_type);
    void UpdateProcesses(const float deltaTime);

private:
    void Detach(const shared_ptr<Process>& process);

    ProcessList processes_;
};

}

#endif /* defined(__kawaii__process_manager__) */
