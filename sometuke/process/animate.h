#ifndef __sometuke__process_animate__
#define __sometuke__process_animate__

#include "sometuke/process/process.h"
#include "sometuke/animation.h"
#include "sometuke/node/sprite.h"

#include <memory>

namespace sometuke {
    
class Animate : public Process {
public:
    static const HashedString TYPE;
    Animate(const weak_ptr<Sprite>& target, const shared_ptr<Animation>& animation)
        : target_(target),
          animation_(animation),
          frame_num_(0),
          elapsed_(0),
          executed_loops_(0),
          original_frame_(NULL, Rect(0, 0, 0, 0)) {
        set_target(target);
    }

    const HashedString& type() const {
        return Animate::TYPE;
    }

    const bool sleeping() {
        return !target_.expired() && target_.lock()->paused();
    }

    void OnStart();
    void OnEnd();
    bool Update(const s2_time delta);
    
    virtual void set_target(const weak_ptr<Sprite>& target_weak);

    shared_ptr<Sprite> target() {
        return target_.lock();
    }

private:
    unsigned int frame_num_;
    shared_ptr<Animation> animation_;
    s2_time elapsed_;
    unsigned int executed_loops_;

    weak_ptr<Sprite> target_;
    SpriteFrame original_frame_;
};

}

#endif /* defined(__sometuke__process_animate__) */
