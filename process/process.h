#ifndef __kawaii__process_process__
#define __kawaii__process_process__

#include "kawaii/application.h"
#include "kawaii/hashed_string.h"
#include "kawaii/types.h"
#include "kawaii/memory_pool.h"
#include "kawaii/process/process_manager.h"
#include "kawaii/logger.h"

#include <memory>

namespace kawaii {
using namespace std;

class Process : public GeneralPoolable,
                public enable_shared_from_this<Process> {
public:
    Process()
        : running_(false),
          sleeping_(false) {
    }
    
    virtual ~Process() {}

    const bool running() {
        return running_;
    }

    const bool sleeping() const {
        return sleeping_;
    }

    void set_running(bool value) {
        running_ = value;
    }
    void Sleep()  { sleeping_ = true; }
    void Wakeup() { sleeping_ = false; }

    virtual EventDispatcher& dispatcher() const {
        return Application::Instance().director().dispatcher();
    }

    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual bool Update(const ii_time delta) = 0;
    
    virtual const HashedString& type() const = 0;

    shared_ptr<Process> Repeat(int repeat = -1);

    template<class T>
    shared_ptr<Process> Chain();
    
    template<class T, class Arg1, class... Args>
    shared_ptr<Process> Chain(Arg1&& arg1, Args&& ... args);

protected:
    bool running_;
    bool killed_;
    bool sleeping_;

private:
    Process(const Process&);
    Process& operator=(const Process&);
};

}

#endif /* defined(__kawaii__process_process__) */
