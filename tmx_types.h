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

enum class TmxTileFlags {
    HORIZONTAL = 0x80000000,
    VERTICAL   = 0x40000000,
    DIAGONAL   = 0x20000000,
    ALL	       = (HORIZONTAL|VERTICAL|DIAGONAL),
    FLIPPED    = ~(ALL),
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


enum class TmxEncoding {
    CSV,
    XML,
    BASE64,
};

enum class TmxCompression {
    NONE,
    ZLIB,
    GZIP,
};

struct TmxLayerInfo {
    TmxLayerInfo()
        : offset_in_tiles(0, 0),
          opacity(255),
          visible(true) {
    }

    string name;
    ivec2 size_in_tiles;
    ivec2 offset_in_tiles;
    bool visible;
    unsigned char opacity;
    bool tiles_owned;
    vector<tmx_gid> gids;
    tmx_gid gid_min;
    tmx_gid gid_max;
    unordered_map<string, string> properties;
};

struct TmxTilesetInfo {
    TmxTilesetInfo()
        : tile_offset(0, 0),
          spacing(0),
          margin(0) {
    }

    string name;
    tmx_gid first_gid;
    ivec2 size_in_tiles;
    vec2 tile_offset;
    float tile_spacing;
    float tile_margin;
    unsigned int spacing;
    unsigned int margin;
    string image_source;
    ivec2 image_size;
};

struct TmxMapInfo {
    TmxEncoding encoding;
    string filename;
    float version;
    TmxOrientation orientation;

    ivec2 size_in_tiles;
    vec2 tile_size;
    Color3B background_color;

    vector<shared_ptr<TmxLayerInfo> > layers;
    vector<shared_ptr<TmxTilesetInfo> > tilesets;
    vector<shared_ptr<TmxObjectGroup> > object_groups;
    TmxProperties properties;
};

}

#endif /* defined(__sometuke__tmx_types__) */
