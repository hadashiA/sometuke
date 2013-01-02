#ifndef __kawaii__process__
#define __kawaii__process__

#include "hashed_string.h"

#include <memory>

namespace kawaii {
using namespace std;

class Process {
public:
    Process(unsigned int priority=0);
    virtual ~Process();

    const bool killed() const {
        return killed_;
    }

    const bool dead() const {
        return killed_;
    }

    const bool is_active() const {
        return is_active_;
    }

    void Activate() {
        is_active_ = true;
    }

    void Deactivate() {
        is_active_ = false;
    }

    const bool paused() const {
        return paused_;
    }

    const bool initialized() const {
        return initialized_;
    }

    const shared_ptr<Process> next() const {
        return next_;
    }

    const bool IsAttached() const;

    virtual void Initialize() {
        initialized_ = true;
    }

    virtual void Update(const float delta) {
        if (!initialized_) {
            Initialize();
        }
    }
    virtual void Kill();
    virtual void TogglePause() { paused_ = !paused_; }

protected:
    bool killed_;
    bool paused_;
    bool is_active_;
    bool initialized_;
    shared_ptr<Process> next_;

private:
    Process(const Process&);
    Process& operator=(const Process&);

    unsigned int flags;
};

}

#endif /* defined(__kawaii__process__) */
