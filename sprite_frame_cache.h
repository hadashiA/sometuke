#ifndef __kawaii__sprite_frame_cache__
#define __kawaii__sprite_frame_cache__

#include "hashed_string.h"

#include "picojson/picojson.h"

#include <map>
#include <set>
#include <memory>

namespace kawaii {

class SpriteFrame;
class Texture2D;

class SpriteFrameCache {
public:
    static unique_ptr<SpriteFrameCache>& Shared() {
        static unique_ptr<SpriteFrameCache> __shared(new SpriteFrameCache);
        return __shared;
    }

    bool AddSpriteFramesWithFile(const string& json_path);

    shared_ptr<SpriteFrame> get(const char* sprite_frame_name) {
        HashedString hashed_name(sprite_frame_name);
        return sprite_frames_[hashed_name];
    }

    shared_ptr<SpriteFrame> operator[](const char* sprite_frame_name) {
        return get(sprite_frame_name);
    }

private:
    typedef std::map<HashedString, shared_ptr<SpriteFrame> > SpriteFrameTable;

    SpriteFrameCache() {}
    void AddSpriteFrameFromJSON(shared_ptr<Texture2D> texture,
                                const string& name,
                                const picojson::value& frame_json);

    SpriteFrameTable sprite_frames_;
    std::set<HashedString> loaded_filenames_;
};

}

#endif /* defined(__kawaii__sprite_frame_cache__) */
