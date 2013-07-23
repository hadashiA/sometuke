#ifndef __sometuke__process_delay__
#define __sometuke__process_delay__

#include "sometuke/process/interval.h"

namespace sometuke {

class Delay : public Interval {
public:
    static const HashedString TYPE;

    Delay(const s2_time duration)
        : Interval(duration) {
    }

    virtual ~Delay() {}

    virtual const HashedString& type() const {
        return Delay::TYPE;
    }

    virtual bool Progress(const s2_time progress) {
        return true;
    }

private:
};


}

#endif /* defined(__sometuke__process_move_by__) */
