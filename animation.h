#ifndef __kawaii__animation__
#define __kawaii__animation__

#include "types.h"
#include "event_dispatcher.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class SpriteFrame;

struct AnimationFrame {
    AnimationFrame(shared_ptr<SpriteFrame> f, ii_time t, const char *e = "")
        : sprite_frame(f),
          time(t),
          event_type(e) {
    }

    shared_ptr<SpriteFrame> sprite_frame;
    ii_time time;
    EventType event_type;
};

struct Animation {
    explicit Animation(vector<AnimationFrame> f, bool l=false)
        : frame_size(f.size()),
          loops(l),
          frames(f) {
    }

    unsigned int frame_size;
    bool loops;
    vector<AnimationFrame> frames;
};

}

#endif /* defined(__kawaii__animation__) */
