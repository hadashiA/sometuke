#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

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

enum class TmxObjectShape {
    RECT,
    ELLIPSE,
    POLYGON,
    POLYLINE,
};

struct TmxObject {
    TmxObject()
        : shape(TmxObjectShape::RECT),
          gid(0),
          size_in_pixels(0, 0),
          offset_in_tiles(0, 0),
          visible(true),
          rotation(false) {
    }

    TmxObjectShape shape;
    string name;
    string type;
    tmx_gid gid;
    ivec2 offset_in_tiles;
    vec2 size_in_pixels;
    vector<float> vertices;
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
