#ifndef __sometuke__process_move_by__
#define __sometuke__process_move_by__

#include "sometuke/process/interval.h"
#include "sometuke/node/node.h"

namespace sometuke {

class MoveBy : public Interval {
public:
    static const HashedString TYPE;

    MoveBy(shared_ptr<Node> target, const s2_time duration, const vec3 delta)
        : target_(target),
          Interval(duration),
          delta_(delta) {
    }

    const bool sleeping() {
        return !target_.expired() && target_.lock()->paused();
    }

    virtual ~MoveBy() {}

    const HashedString& type() const {
        return MoveBy::TYPE;
    }

    void OnEnter() {
        if (shared_ptr<Node> node = target_.lock()) {
            from_ = node->position();
        }
    }

    bool Progress(const float progress) {
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
    vec3 delta_;
};


}

#endif /* defined(__sometuke__process_move_by__) */
