#ifndef __sometuke__process_move_to__
#define __sometuke__process_move_to__

#include "sometuke/process/interval.h"

#include "sometuke/node/node.h"

#include <memory>

namespace sometuke {

class MoveTo : public Interval {
public:
    static const HashedString TYPE;

    MoveTo(shared_ptr<Node> target, const s2_time duration, const vec3& to)
        : Interval(duration),
          target_(target),
          to_(to) {
    }

    virtual ~MoveTo() {}

    const bool sleeping() {
        return !target_.expired() && target_.lock()->paused();
    }

    const HashedString& type() const {
        return MoveTo::TYPE;
    }

    void OnStart() {
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

#endif /* defined(__sometuke__process_move_to__) */
