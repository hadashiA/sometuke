#ifndef __kawaii__process_repeat__
#define __kawaii__processr_epeat__

#include "process.h"

#include <memory>

namespace kawaii {

class Repeat : public kawaii::Process {
public:
    static const HashedString TYPE;

    Repeat(shared_ptr<Process> inner_process, int repeat = -1)
        : inner_process_(inner_process),
          repeat_(repeat),
          num_repeated_(0) {
    }

    virtual void OnEnter() {
        num_repeated_ = 0;
        inner_process_->OnEnter();
    }

    virtual void Update(const ii_time delta) {
        if (!inner_process_->dead()) {
            inner_process_->Update(delta);
        }

        if (inner_process_->dead()) {
            if (is_forever() || ++num_repeated_ < repeat_) {
                inner_process_->OnEnter();
            } else {
                Kill();
            }
        }
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

#endif /* defined(__kawaii__process_repeat__) */
