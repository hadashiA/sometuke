#ifndef __kawaii__animation__
#define __kawaii__animation__

#include "kawaii/types.h"
#include "kawaii/event_dispatcher.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class SpriteFrame;

struct AnimationFrame {
    AnimationFrame(shared_ptr<SpriteFrame> f, ii_time t, const char *e = "")
        : sprite_frame(f),
          time(t) {
    }

    shared_ptr<SpriteFrame> sprite_frame;
    ii_time time;
};

struct Animation {
    explicit Animation(vector<AnimationFrame> f, unsigned int l=1)
        : frame_size(f.size()),
          loops(l),
          frames(f) {
    }

    unsigned int frame_size;
    unsigned int loops;
    vector<AnimationFrame> frames;
};

}

#endif /* defined(__kawaii__animation__) */
