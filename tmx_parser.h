#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

#include "sometuke/vector.h"

#include <string>
#include <array>
#include <unordered_map>

namespace sometuke {
using namespace std;

enum class TmxFormat {
    CSV,
    XML,
    BASE64,
    GZIP,
    ZLIB,
};

enum class TmxNode {
    MAP,
    LAYER,
    OBJECT_GROUP,
    OBJECT,
    TILE,
};

struct TmxLayerInfo {
    TmxLayerInfo(size_t num_tiles)
        : gids(num_tiles) {
    }

    string name;
    ivec2 tiles_size;
    vec2 offset;
    bool visible;
    bool tiles_owned;
    array<unsigned int> gids;
    unsigned int gid_min;
    unsigned int gid_max;
    unordered_map<string, string> properties;
};

struct TmxTilesetInfo {
    string name;
    unsigned int first_gid;
    vec2 size;
    unsigned int spacing;
    unsigned int margin;
    string source_image;
    vec2 image_size;
};

struct TmxMapInfo {
    TmxFormat format;
    string current_string;
    bool storing_characters;
    int parent_element;
    unsigned int parent_gid;
    string filename;
    int orientation;
    vec2 map_size;
    ivec2 tiles_size;

    vector<TmxLayerInfo> layers;
    vector<TmxTilesetInfo> tilesets;
    unordered_map<string, string> properties;
};

}

#endif /* defined(__sometuke__tmx_parser__) */
