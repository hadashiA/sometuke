#ifndef __kawaii__process_interval__
#define __kawaii__process_interval__

#include "kawaii/process/process.h"

#include <algorithm>
#include <cfloat>

namespace kawaii {
using std::min;
using std::max;

class Interval : public Process {
public:
    Interval(ii_time duration = FLT_EPSILON)
        : duration_(duration) {
    }

    virtual ~Interval() {}

    virtual void Start() {
        elapsed_ = 0;
    }

    virtual bool Update(const ii_time delta) {
        elapsed_ += delta;
        // percentage of duration
        float progress = elapsed_ / max<float>(duration_, FLT_EPSILON);
        progress = max<float>(0, min<float>(1, progress));
        bool continued = Progress(progress);
        if (continued && elapsed_ < duration_) {
            return true;
        }
        return false;
    }

    virtual bool Progress(const float progress) = 0;

protected:
    ii_time duration_;
    ii_time elapsed_;
};

}

#endif /* defined(__kawaii__process_interval__) */
