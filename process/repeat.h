#ifndef __sometuke__process_repeat__
#define __sometuke__processr_epeat__

#include "sometuke/process/process.h"
#include "sometuke/logger.h"

#include <memory>

namespace sometuke {

class Repeat : public Process {
public:
    static const HashedString TYPE; // = "process:repeat";

    Repeat(const shared_ptr<Process>& inner_process, const int& repeat = -1)
        : inner_process_(inner_process),
          repeat_(repeat),
          num_repeated_(0) {
    }

    virtual const HashedString& type() const {
        return Repeat::TYPE;
    }

    virtual void OnEnter() {
        num_repeated_ = 0;
        inner_process_->Enter();
    }

    virtual void OnExit() {
        inner_process_->Exit();
    }

    virtual bool Update(const s2_time delta) {
        bool continued = inner_process_->Update(delta);
        
        if (continued) {
            return true;
        } else {
            if (is_forever() || ++num_repeated_ < repeat_) {
                inner_process_->Enter();
                return true;
            }
        }
        return false;
    }

    bool is_forever() const {
        return repeat_ < 0;
    }

private:
    shared_ptr<Process> inner_process_;
    int repeat_;
    unsigned int num_repeated_;
};

}

#endif /* defined(__sometuke__process_repeat__) */
