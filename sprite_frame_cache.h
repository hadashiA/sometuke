#ifndef __kawaii__sprite_frame_cache__
#define __kawaii__sprite_frame_cache__

#include "hashed_string.h"

#include <map>

namespace kawaii {

class SpriteFrame;

class SpriteFrameCache {
public:
    static inline SpriteFrameCache *Shared() {
        if (__shared == NULL) {
            __shared = new SpriteFrameCache;
        }
        return __shared;
    }

    bool AddSpriteFramesWithFile(const string& json_path);

private:
    typedef std::map<unsigned int, shared_ptr<SpriteFrame> > SpriteFrameTable;

    static SpriteFrameCache *__shared;

    SpriteFrameCache() {}

    

    SpriteFrameTable sprite_frames_;
};

}

#endif /* defined(__kawaii__sprite_frame_cache__) */
