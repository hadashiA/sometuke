#ifndef __skidarak__sprite_frame_cache__
#define __skidarak__sprite_frame_cache__

#include "skidarak/hashed_string.h"

#include "skidarak/picojson/picojson.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace skidarak {

class SpriteFrame;
class Texture2D;

class SpriteFrameCache {
public:
    static SpriteFrameCache& Instance() {
        static unique_ptr<SpriteFrameCache> __shared(new SpriteFrameCache);
        return *__shared;
    }

    bool AddSpriteFramesWithFile(const string& json_path);

    shared_ptr<SpriteFrame> get(const char* sprite_frame_name) {
        HashedString hashed_name(sprite_frame_name);
        return sprite_frames_[hashed_name.id()];
    }

    shared_ptr<SpriteFrame> operator[](const char* sprite_frame_name) {
        return get(sprite_frame_name);
    }

private:
    typedef unordered_map<unsigned long, shared_ptr<SpriteFrame> > SpriteFrameTable;

    SpriteFrameCache() {}

    SpriteFrameCache(const SpriteFrameCache&);
    SpriteFrameCache& operator=(const SpriteFrameCache&);

    void AddSpriteFrameFromJSON(shared_ptr<Texture2D> texture,
                                const string& name,
                                const picojson::value& frame_json);

    SpriteFrameTable sprite_frames_;
    unordered_set<unsigned long> loaded_filenames_;
};

}

#endif /* defined(__skidarak__sprite_frame_cache__) */
