#include "animate.h"
#include "sprite.h"

namespace kawaii {

const HashedString Animate::TYPE("animate process");

bool Animate::PreUpdate() {
    target_->set_display_frame(animation_->frames[frame_num_].sprite_frame);

    return true;
}

bool Animate::Update(ii_time delta_time) {
    ii_time current_frame_time = animation_->frames[frame_num_].time;
    if (elapsed_ > current_frame_time) {
        frame_num_ = (frame_num_ + 1) % animation_->frame_size;
        elapsed_ = 0;

        shared_ptr<SpriteFrame> sprite_frame = animation_->frames[frame_num_].sprite_frame;
        target_->set_display_frame(sprite_frame);
    }
    elapsed_ += delta_time;
    return true;
}

}
