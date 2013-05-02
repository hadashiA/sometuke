#ifndef __kawaii__process_process__
#define __kawaii__process_process__

#include "kawaii/hashed_string.h"
#include "kawaii/types.h"
#include "kawaii/memory_pool.h"
#include "kawaii/process/process_scheduler.h"
#include "kawaii/application.h"

#include <memory>

namespace kawaii {
using namespace std;

class Process : public GeneralPoolable,
                public enable_shared_from_this<Process> {
public:
    // typedef enum {
    //     ATTACHED = 0x00000001,
    // } Flags;

    Process()
        : killed_(false),
          paused_(false),
          initialized_(false) {
    }
    
    virtual ~Process() {}

    const bool killed() const {
        return killed_;
    }

    const bool dead() const {
        return killed_;
    }

    const bool paused() const {
        return paused_;
    }

    const bool initialized() const {
        return initialized_;
    }

    virtual bool Visit(const ii_time delta_time) {
        if (!initialized_) {
            OnEnter();
            initialized_ = true;
        }
        if (dead()) {
            return false;
        }
        if (paused()) {
            return true;
        }
        return Update(delta_time);
    }
    
    virtual ProcessScheduler& scheduler(){
        return Application::Instance().director().scheduler();
    }

    void Kill()   { killed_ = true; }
    void Pause()  { paused_ = true; }
    void Resume() { paused_ = false; }

    void Run() { scheduler().Attach(shared_from_this()); }

    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual bool Update(const ii_time delta) = 0;
    
    virtual const HashedString& type() const = 0;

protected:
    bool killed_;
    bool paused_;
    bool sleeping_;
    bool initialized_;

private:
    Process(const Process&);
    Process& operator=(const Process&);

    unsigned int flags;
};

}

#endif /* defined(__kawaii__process_process__) */
