#ifndef __kawaii__process_animate__
#define __kawaii__process_animate__

#include "kawaii/process/process.h"
#include "kawaii/animation.h"
#include "kawaii/node/sprite.h"

#include <memory>

namespace kawaii {
    
class Sprite;

class Animate : public Process {
public:
    static const HashedString TYPE;
    Animate(weak_ptr<Sprite> target, shared_ptr<Animation> animation) :
        animation_(animation),
        frame_num_(0),
        elapsed_(0),
        target_(target),
        original_frame_(target.lock()->display_frame()),
        executed_loops_(0) {
    }

    virtual const HashedString& type() const {
        return Animate::TYPE;
    }

    virtual void OnEnter() {
        frame_num_ = 0;
        elapsed_ = 0;
        executed_loops_ = 0;
        killed_ = false;
        if (shared_ptr<Sprite> sprite = target_.lock()) {
            sprite->set_display_frame(animation_->frames[frame_num_].sprite_frame);
        }
    }
    
    virtual void OnExit() {
        if (shared_ptr<Sprite > sprite = target_.lock()) {
            sprite->set_display_frame(original_frame_);
        }
    }

    virtual bool Update(const ii_time delta);

private:
    weak_ptr<Sprite> target_;
    unsigned int frame_num_;
    shared_ptr<Animation> animation_;
    ii_time elapsed_;
    unsigned int executed_loops_;

    SpriteFrame original_frame_;
};

}

#endif /* defined(__kawaii__process_animate__) */
