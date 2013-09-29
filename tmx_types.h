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
    TmxObject()
        : visible(true),
          rotation(false) {
    }

    string name;
    string type;
    tmx_gid gid;
    ivec2 offset_in_tiles;
    vec2 size_in_pixels;
    bool rotation;
    bool visible;
    TmxProperties properties;
};

struct TmxObjectGroup {
    TmxObjectGroup()
        : offset_in_tiles(0, 0),
          visible(true) {
    }

    string name;
    ivec2 offset_in_tiles;
    ivec2 size_in_tiles;
    bool visible;
    float opacity;
    vector<TmxObject> objects;
};

}

#endif /* defined(__sometuke__tmx_types__) */
