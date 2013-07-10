#ifndef __skidarak__process_move_to__
#define __skidarak__process_move_to__

#include "skidarak/process/interval.h"

#include "skidarak/node/node.h"
#include "skidarak/weak_ptr_hash.h"

#include <memory>

namespace skidarak {

class MoveTo : public Interval {
public:
    static const HashedString TYPE;

    MoveTo(shared_ptr<Node> target, const ii_time duration, const vec3& to)
        : Interval(duration),
          target_(target),
          to_(to) {
    }

    virtual ~MoveTo() {}

    virtual const HashedString& type() const {
        return MoveTo::TYPE;
    }

    virtual void OnEnter() {
        Interval::OnEnter();

        if (shared_ptr<Node> node = target_.lock()) {
            from_  = node->position();
            delta_ = to_ - from_;
        }
    }

    virtual bool Progress(const float progress) {
        if (shared_ptr<Node> node = target_.lock()) {
            vec3 pos = from_ + (delta_ * progress);
            node->set_position(pos);
            return true;
        }
        return false;
    }

private:
    weak_ptr<Node> target_;
    vec3 from_;
    vec3 to_;
    vec3 delta_;
};

}

#endif /* defined(__skidarak__process_move_to__) */
