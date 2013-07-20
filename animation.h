#ifndef __sometuke__animation__
#define __sometuke__animation__

#include "sometuke/types.h"
#include "sometuke/event_dispatcher.h"
#include "sometuke/sprite_frame_cache.h"
#include "sometuke/logger.h"

#include <vector>
#include <memory>
#include <string>

namespace sometuke {
using namespace std;

struct AnimationFrame {
    AnimationFrame(shared_ptr<SpriteFrame> f, ii_time t, shared_ptr<Event> e = NULL)
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

    explicit Animation(unsigned int l=1)
        : frame_size(),
          loops(l) {
    }

    void Push(const char *sprite_frame_name, ii_time t, shared_ptr<Event> e = NULL) {
        SpriteFrameCache& cache = SpriteFrameCache::Instance();
        shared_ptr<SpriteFrame> sprite_frame = cache[sprite_frame_name];
        if (!sprite_frame) {
            S2ERROR("sprite frame name %s not found", sprite_frame_name);
        }

        AnimationFrame frame(sprite_frame, t, e);
        frames.push_back(frame);
        frame_size = frames.size();
    }

    unsigned int frame_size;
    unsigned int loops;
    vector<AnimationFrame> frames;
};

}

#endif /* defined(__sometuke__animation__) */
