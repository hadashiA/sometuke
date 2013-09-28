#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

#include "sometuke/vector.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace sometuke {
using namespace std;

typedef uint32_t tmx_gid;

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

struct TmxLayerInfo {
    TmxLayerInfo()
        : offset(0, 0),
          opacity(255),
          visible(true) {
    }

    string name;
    ivec2 num_tiles;
    ivec2 offset;
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
    ivec2 tile_size;
    ivec2 tile_offset;
    ivec2 tile_anchor_point;
    unsigned int spacing;
    unsigned int margin;
    string image_source;
    ivec2 image_size;
};

struct TmxObject {
    string name;
    tmx_gid gid;
    int x;
    int y;
    int width;
    int height;
    unordered_map<string, string> properties;
};

struct TmxObjectGroup {
    string name;
    int width;
    int height;
    bool visible;
    vector<TmxObject> objects;
};

struct TmxMapInfo {
    TmxEncoding encoding;
    string current_string;
    bool storing_characters;
    int parent_element;
    tmx_gid parent_gid;
    string filename;
    TmxOrientation orientation;
    ivec2 map_size;
    ivec2 tile_size;

    vector<TmxLayerInfo> layers;
    vector<TmxTilesetInfo> tilesets;
    vector<TmxObjectGroup> object_groups;
    unordered_map<string, string> properties;
};

class TmxParser {
public:
    static TmxParser& Instance() {
        static unique_ptr<TmxParser> __instance(new TmxParser);
        return *__instance;
    }

    shared_ptr<TmxMapInfo> Parse(const string& file);

private:
    TmxParser() {}

    TmxParser(const TmxParser&) = delete;
    TmxParser(TmxParser&&) = delete;
    TmxParser& operator=(const TmxParser&) = delete;
    TmxParser& operator=(TmxParser&&) = delete;

    bool ParseLayerData(TmxLayerInfo &layer_info,
                        const string data,
                        TmxEncoding encoding,
                        TmxCompression compression);
};

}

#endif /* defined(__sometuke__tmx_parser__) */
