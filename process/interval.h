#ifndef __kawaii__process_interval__
#define __kawaii__process_interval__

#include "process.h"

namespace kawaii {

class Interval : public Process {
public:
    virtual bool Visit(const ii_time delta) {
        if (!initialized_) {
            OnEnter();
            initialized_ = true;
        }
        if (!dead()) {
            

        }
        return false;
    }

private:
    ii_time duration_;
    ii_time elapsed_;
};

}

#endif /* defined(__kawaii__process_interval__) */
