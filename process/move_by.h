#ifndef __kawaii__process_move_by__
#define __kawaii__process_move_by__

#include "interval.h"

#include "node.h"

namespace kawaii {

class MoveBy : public Interval {
public:
    static const HashedString TYPE;

    MoveBy(weak_ptr<Node> target, const ii_time duration, const vec3 delta)
        : Interval(duration),
          target_(target),
          delta_(delta) {

    }

    virtual ~MoveBy() {}

    virtual const HashedString& type() const {
        return MoveBy::TYPE;
    }

    virtual void OnEnter() {
        if (shared_ptr<Node> node = target_.lock()) {
            from_ = node->local_position();
        }
    }

    virtual bool Update(const ii_time progress) {
        if (shared_ptr<Node> node = target_.lock()) {
            vec3 pos = from_ + (delta_ * progress);
            node->set_local_position(pos);
            return true;
        }
        return false;
    }

private:
    weak_ptr<Node> target_;
    vec3 from_;
    vec3 delta_;
};


}

#endif /* defined(__kawaii__process_move_by__) */
