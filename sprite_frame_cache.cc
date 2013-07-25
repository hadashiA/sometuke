#include "sometuke/sprite_frame_cache.h"

#include "sometuke/sprite_frame.h"
#include "sometuke/director.h"
#include "sometuke/application_component.h"
#include "sometuke/texture_cache.h"
#include "sometuke/logger.h"

#include <fstream>
#include <memory>
#include <string>
#include <cstdlib>
#include <cassert>

namespace sometuke {
using namespace std;

bool SpriteFrameCache::AddSpriteFramesWithFile(const string& json_path) {
    HashedString hashed_json_path(json_path);
    if (loaded_filenames_.find(hashed_json_path.id()) != loaded_filenames_.end()) {
        return true;
    }

    loaded_filenames_.insert(hashed_json_path.id());

    AssetsLoader& loader = Director::Instance().loader();
    const string& path = loader.FullPathFromRelativePath(json_path);
    ifstream io(path);

    picojson::value json;
    io >> json;

    if (!json.is<picojson::object>()) {
        IIWARN("%s is not a json object:%s", path.c_str(), json.to_str().c_str());
        return false;
    }

    const picojson::value& metadata = json.get("meta");
    if (!metadata.is<picojson::object>()) {
        IIWARN("metadata property not found:%s", metadata.to_str().c_str());
        return false;
    }

    int format = 0;
    const picojson::value& version = metadata.get("version");
    if (version.is<string>()) {
        const string& version_str = version.get<string>();
        format = atoi(version_str.c_str());
    }
    // if (format < 0 || format > 3) {
    if (format != 1) {
        IIWARN("non support format:%d", format);
        return false;
    }

    const string& texture_filename = metadata.get("image").get<string>();
    shared_ptr<Texture2D> texture = TextureCache::Instance().FetchFromPath(texture_filename);

    picojson::value frames = json.get("frames");
    if (frames.is<picojson::array>()) {
        const picojson::array& frames_array = frames.get<picojson::array>();
        for (picojson::array::const_iterator iter = frames_array.begin();
             iter != frames_array.end();
             ++iter) {
            
            const picojson::value& frame_json = *iter;
            const string& name = frame_json.get("filename").get<string>();
            AddSpriteFrameFromJSON(texture, name, frame_json);
        }
        
    } else if (frames.is<picojson::object>()) {
        const picojson::object& frames_object = frames.get<picojson::object>();
        for (picojson::object::const_iterator iter = frames_object.begin();
             iter != frames_object.end();
             ++iter) {

            const string& name = iter->first;
            const picojson::value& frame_json = iter->second;
            AddSpriteFrameFromJSON(texture, name, frame_json);
        }

    } else {
        IIWARN("frames property not array:%s", frames.to_str().c_str());
        return false;
    }
    
    return true;
}

void SpriteFrameCache::AddSpriteFrameFromJSON(shared_ptr<Texture2D> texture,
                                              const string& name,
                                              const picojson::value& frame_json) {
    const picojson::value& frame_rect_json    = frame_json.get("frame");
    const picojson::value& original_size_json = frame_json.get("sourceSize");
    bool rotated = frame_json.get("rotated").get<bool>();
    // bool trimmed = frame_json.get("trimmed").get<bool>();
    
    Rect frame_rect(frame_rect_json.get("x").get<double>(),
                    frame_rect_json.get("y").get<double>(),
                    frame_rect_json.get("w").get<double>(),
                    frame_rect_json.get("h").get<double>()
                    );
    vec2 original_size(original_size_json.get("w").get<double>(),
                       original_size_json.get("h").get<double>()
                       );
    
    shared_ptr<SpriteFrame> sprite_frame(new SpriteFrame(texture,
                                                         frame_rect,
                                                         rotated,
                                                         vec2(0, 0),
                                                         original_size,
                                                         name
                                                         ));
    HashedString hashed_name(name);
    sprite_frames_[hashed_name.id()] = sprite_frame;
}

}
