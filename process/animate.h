#ifndef __kawaii__process_animate__
#define __kawaii__process_animate__

#include "kawaii/process/process.h"
#include "kawaii/animation.h"
#include "kawaii/node/sprite.h"

#include <memory>

namespace kawaii {
    
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

    virtual const HashedString& type() const {
        return Animate::TYPE;
    }

    virtual void set_target(weak_ptr<Sprite> target_weak);

    virtual void OnEnter();
    virtual void OnExit();
    virtual bool Update(const ii_time delta);
    
    shared_ptr<Sprite> target() {
        return target_.lock();
    }

private:
    unsigned int frame_num_;
    shared_ptr<Animation> animation_;
    ii_time elapsed_;
    unsigned int executed_loops_;

    weak_ptr<Sprite> target_;
    SpriteFrame original_frame_;
};

}

#endif /* defined(__kawaii__process_animate__) */
