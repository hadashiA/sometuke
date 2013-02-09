#include "sprite_frame_cache.h"

#include "sprite_frame.h"
#include "application.h"
#include "assets.h"
#include "logger.h"

#include "picojson/picojson.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

namespace kawaii {
using namespace std;

SpriteFrameCache *SpriteFrameCache::__shared = NULL;

bool SpriteFrameCache::AddSpriteFramesWithFile(const string& json_path) {
    Assets *assets = Application::Current()->assets();
    const string& path = assets->FullPathFromRelativePath(json_path);
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

    IIINFO("format:%d", format);

    picojson::value frames = json.get("frames");
    
    
    return true;
}

}
