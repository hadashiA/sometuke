#ifndef __sometuke__process_interval__
#define __sometuke__process_interval__

#include "sometuke/process/process.h"

#include <algorithm>
#include <cfloat>

namespace sometuke {
using std::min;
using std::max;

class Interval : public Process {
public:
    Interval(s2_time duration = FLT_EPSILON)
        : duration_(duration) {
    }

    virtual ~Interval() {}

    void Start() {
        Process::Start();
        elapsed_ = 0;
    }

    bool Update(const s2_time delta) {
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
    s2_time duration_;
    s2_time elapsed_;
};

}

#endif /* defined(__sometuke__process_interval__) */
