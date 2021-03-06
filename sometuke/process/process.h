#ifndef __sometuke__process_process__
#define __sometuke__process_process__

#include "sometuke/director.h"
#include "sometuke/hashed_string.h"
#include "sometuke/types.h"
#include "sometuke/memory_pool.h"
#include "sometuke/process/process_manager.h"
#include "sometuke/logger.h"

#include <memory>

namespace sometuke {
using namespace std;

class Sequence;

class Process : public enable_shared_from_this<Process> {
public:
    Process() {}
    virtual ~Process() {}

    virtual const bool sleeping() { return false; }

    void Start() {
        OnStart();
    }

    void End() {
        OnEnd();
    }

    virtual bool Update(const s2_time delta) = 0;
    
    virtual const HashedString& type() const = 0;

    shared_ptr<Process> Repeat(int repeat = -1);
    shared_ptr<Process> Delay(const s2_time duration);
    shared_ptr<Process> Interval(const s2_time interval);
    shared_ptr<Process> Timer(const s2_time interval,
                              const unsigned int repeat, const s2_time delay);
    template<class T>
    shared_ptr<Sequence> Chain();
    
    template<class T, class Arg1, class... Args>
    shared_ptr<Sequence> Chain(Arg1&& arg1, Args&& ... args);

protected:
    virtual void OnStart() {}
    virtual void OnEnd() {}

private:
    Process(const Process&);
    Process& operator=(const Process&);
};

}

#endif /* defined(__sometuke__process_process__) */
