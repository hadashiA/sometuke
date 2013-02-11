#ifndef __kawaii__animation__
#define __kawaii__animation__

#include "types.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class SpriteFrame;

struct AnimationFrame {
    shared_ptr<SpriteFrame> sprite_frame;
    ii_time time;
};

struct Animation {
    unsigned int frame_size;
    bool loops;
    vector<AnimationFrame> frames;
};

}

#endif /* defined(__kawaii__animation__) */
