#ifndef __kawaii__process__
#define __kawaii__process__

#include "hashed_string.h"

#include <memory>

namespace kawaii {
using namespace std;

class Process {
public:
    // typedef enum {
    //     ATTACHED = 0x00000001,
    // } Flags;

    Process()
        : killed_(false),
          paused_(false),
          is_active_(true),
          initialized_(false),
          next_(NULL) {
    }
    
    virtual ~Process() {}

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

    void set_next(shared_ptr<Process> value) {
        next_ = value;
    }

    virtual void Initialize() {
        initialized_ = true;
    }

    virtual void Update(const float delta) {
        if (!initialized_) {
            Initialize();
        }
    }
    virtual void Kill()   { killed_ = true; }
    virtual void Pauce()  { paused_ = true; }
    virtual void Resume() { paused_ = false; }
    virtual const HashedString& type() = 0;

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
