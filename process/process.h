#ifndef __kawaii__process_process__
#define __kawaii__process_process__

#include "kawaii/application.h"
#include "kawaii/hashed_string.h"
#include "kawaii/types.h"
#include "kawaii/memory_pool.h"
#include "kawaii/process/process_manager.h"
#include "kawaii/node/node.h"
#include "kawaii/logger.h"

#include <memory>

namespace kawaii {
using namespace std;

class Process : public GeneralPoolable,
                public enable_shared_from_this<Process> {
public:
    Process()
        : killed_(false),
          sleeping_(false) {
    }
    
    virtual ~Process() {}

    const bool killed() const {
        return killed_;
    }

    bool dead() const {
        return killed_;
    }

    bool sleeping() const {
        return sleeping_;
    }

    virtual bool Update(const ii_time delta_time) {
        if (killed_) {
            return false;
        }
        if (sleeping_) {
            return true;
        }
        return Step(delta_time);
    }
    
    void Kill()   { killed_ = true; }
    void Sleep()  { sleeping_ = true; }
    void Wakeup() { sleeping_ = false; }

    void StartWithTarget(weak_ptr<Node> target) {
        set_target(target);
        Start();
    }
    virtual void set_target(weak_ptr<Node> target) {
        target_ = target;
    }
    virtual void Start() {}
    virtual void End() {}
    virtual bool Step(const ii_time delta) = 0;
    
    virtual const HashedString& type() const = 0;

protected:
    weak_ptr<Node> target_;
    bool killed_;
    bool paused_;
    bool sleeping_;

private:
    Process(const Process&);
    Process& operator=(const Process&);
};

}

#endif /* defined(__kawaii__process_process__) */
