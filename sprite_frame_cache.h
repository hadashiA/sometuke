#ifndef __sometuke__sprite_frame_cache__
#define __sometuke__sprite_frame_cache__

#include "sometuke/hashed_string.h"

#include "sometuke/picojson/picojson.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace sometuke {

class SpriteFrame;
class Texture2D;

class SpriteFrameCache {
public:
    static SpriteFrameCache& Instance() {
        static unique_ptr<SpriteFrameCache> __shared(new SpriteFrameCache);
        return *__shared;
    }

    bool AddSpriteFramesWithFile(const string& json_path);
    void Purge();

    shared_ptr<SpriteFrame> get(const string& sprite_frame_name) {
        return sprite_frames_[sprite_frame_name];
    }

    shared_ptr<SpriteFrame> operator[](const string& sprite_frame_name) {
        return get(sprite_frame_name);
    }

private:
    typedef unordered_map<string, shared_ptr<SpriteFrame> > SpriteFrameTable;
    
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

#endif /* defined(__sometuke__sprite_frame_cache__) */
