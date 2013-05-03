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
    explicit Animate(shared_ptr<Animation> animation) :
        animation_(animation),
        frame_num_(0),
        elapsed_(0),
        executed_loops_(0),
        original_frame_(NULL, Rect(0, 0, 0, 0)) {
    }

    virtual const HashedString& type() const {
        return Animate::TYPE;
    }

    virtual void set_target(weak_ptr<Node> target_weak) {
        shared_ptr<Node> target = target_weak.lock();
        if (target) {
            shared_ptr<Sprite> sprite = static_pointer_cast<Sprite>(target);
            if (sprite != target_sprite()) {
                original_frame_ = sprite->display_frame();
            }
        }
        target_ = target;
    }

    virtual void Start() {
        frame_num_ = 0;
        elapsed_ = 0;
        executed_loops_ = 0;
        killed_ = false;
        if (shared_ptr<Sprite> sprite = target_sprite()) {
            sprite->set_display_frame(animation_->frames[frame_num_].sprite_frame);
        }
    }
    
    virtual void End() {
        if (shared_ptr<Sprite > sprite = target_sprite()) {
            sprite->set_display_frame(original_frame_);
        }
    }

    virtual bool Step(const ii_time delta);

    shared_ptr<Sprite> target_sprite() {
        return static_pointer_cast<Sprite>(target_.lock());
    }

private:
    unsigned int frame_num_;
    shared_ptr<Animation> animation_;
    ii_time elapsed_;
    unsigned int executed_loops_;

    SpriteFrame original_frame_;
};

}

#endif /* defined(__kawaii__process_animate__) */
