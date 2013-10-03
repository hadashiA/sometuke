#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

#include "sometuke/tmx_types.h"
#include "sometuke/vector.h"
#include "sometuke/color.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace sometuke {
using namespace std;

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

    vector<TmxLayerInfo> layers;
    vector<TmxTilesetInfo> tilesets;
    vector<TmxObjectGroup> object_groups;
    TmxProperties properties;
};

class TmxParser {
public:
    static TmxParser& Instance() {
        static unique_ptr<TmxParser> __instance(new TmxParser);
        return *__instance;
    }

    shared_ptr<TmxTiledMap> Parse(const string& file);

private:
    TmxParser() {}

    TmxParser(const TmxParser&) = delete;
    TmxParser(TmxParser&&) = delete;
    TmxParser& operator=(const TmxParser&) = delete;
    TmxParser& operator=(TmxParser&&) = delete;

    bool ParseLayerData(TmxLayerInfo &layer_info,
                        const string& data,
                        TmxEncoding encoding,
                        TmxCompression compression);
};

}

#endif /* defined(__sometuke__tmx_parser__) */
