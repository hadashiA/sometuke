#ifndef __kawaii__process_delay__
#define __kawaii__process_delay__

#include "kawaii/process/interval.h"

namespace kawaii {

class Delay : public Interval {
public:
    static const HashedString TYPE;

    Delay(const ii_time duration)
        : Interval(duration) {
    }

    virtual ~Delay() {}

    virtual const HashedString& type() const {
        return Delay::TYPE;
    }

    virtual bool Progress(const ii_time progress) {
        return true;
    }

private:
};


}

#endif /* defined(__kawaii__process_move_by__) */
