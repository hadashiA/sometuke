#ifndef __kawaii__process_move_by__
#define __kawaii__process_move_by__

#include "kawaii/process/interval.h"
#include "kawaii/logger.h"

namespace kawaii {

class MoveBy : public Interval {
public:
    static const HashedString TYPE;

    MoveBy(const ii_time duration, const vec3 delta)
        : Interval(duration),
          delta_(delta) {

    }

    virtual ~MoveBy() {}

    virtual const HashedString& type() const {
        return MoveBy::TYPE;
    }

    virtual void Start() {
        Interval::Start();
        
        if (shared_ptr<Node> node = target_.lock()) {
            from_ = node->position();
        }
    }

    virtual bool Step(const ii_time progress) {
        if (shared_ptr<Node> node = target_.lock()) {
            vec3 pos = from_ + (delta_ * progress);
            node->set_position(pos);
            return true;
        }
        return false;
    }

private:
    vec3 from_;
    vec3 delta_;
};


}

#endif /* defined(__kawaii__process_move_by__) */
