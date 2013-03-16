#include "animate.h"
#include "sprite.h"

#include "logger.h"

namespace kawaii {

const HashedString Animate::TYPE("process:animate");

void Animate::Update(ii_time delta_time) {
    ii_time current_frame_time = animation_->frames[frame_num_].time;
    if (elapsed_ > current_frame_time) {
        elapsed_ = 0;
        
        if (++frame_num_ >= animation_->frame_size) {
            if (++executed_loops_ >= animation_->loops) {
                Kill();
                return;
            }
            frame_num_ = 0;
        }

        shared_ptr<SpriteFrame> sprite_frame = animation_->frames[frame_num_].sprite_frame;
        target_->set_display_frame(sprite_frame);
    }
    elapsed_ += delta_time;
}

}
