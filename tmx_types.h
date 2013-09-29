#ifndef __sometuke__tmx_types__
#define __sometuke__tmx_types__

namespace sometuke {

typedef uint32_t tmx_gid;

typedef unordered_map<string, string> TmxProperties;

enum class TmxNode {
    MAP,
    LAYER,
    OBJECT_GROUP,
    OBJECT,
    TILE,
};

enum class TmxOrientation {
    ORTHO,
    HEX,
    ISO,
};

struct TmxObject {
    string name;
    tmx_gid gid;
    ivec2 offset_in_tiles;
    vec2 size_in_pixels;
    TmxProperties properties;
};

struct TmxObjectGroup {
    string name;
    ivec2 offset_in_tiles;
    ivec2 size_in_tiles;
    bool visible;
    float opacity;
    vector<TmxObject> objects;
};

}

#endif /* defined(__sometuke__tmx_types__) */
