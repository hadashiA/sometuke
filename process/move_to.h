#ifndef __kawaii__process_move_to__
#define __kawaii__process_move_to__

#include "kawaii/process/interval.h"

#include "kawaii/node/node.h"
#include "kawaii/weak_ptr_hash.h"

#include <memory>

namespace kawaii {

class MoveTo : public Interval {
public:
    static const HashedString TYPE;

    MoveTo(const ii_time duration, const vec3& to)
        : Interval(duration),
          to_(to) {
    }

    virtual ~MoveTo() {}

    virtual const HashedString& type() const {
        return MoveTo::TYPE;
    }

    virtual void Start() {
        Interval::Start();

        if (shared_ptr<Node> node = target_.lock()) {
            from_  = node->position();
            delta_ = to_ - from_;
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
    vec3 to_;
    vec3 delta_;
};

}

#endif /* defined(__kawaii__process_move_to__) */
