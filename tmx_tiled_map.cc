#include "sometuke/tmx_tiled_map.h"

#include "sometuke/tmx_parser.h"
#include "sometuke/logger.h"
#include "sometuke/node/tmx_layer.h"

#include <arpa/inet.h>

namespace sometuke {

bool TmxTiledMap::InitWithTmxFile(const string& file) {
    const shared_ptr<TmxMapInfo>& info = TmxParser::Instance().Parse(file);
    if (info->tilesets.empty()) {
        S2ERROR("TmxTiledMap: tileset empty");
        return false;
    }

    size_in_tiles_ = info->size_in_tiles;
    tile_size_     = info->tile_size;
    orientation_   = info->orientation;
    object_groups_ = info->object_groups;
    properties_    = info->properties;

    for (auto layer_info : info->layers) {
        const shared_ptr<TmxTilesetInfo>& tileset_info =
            TilesetForLayer(layer_info, info);

        auto layer = make_shared<TmxLayer>();
        if (!layer->InitWithTilesetInfo(tileset_info, layer_info, info)) {
            S2ERROR("TmxTiledMap: faild layer initalize %s", layer_info->name.c_str());
            return false;
        }
    }
}

shared_ptr<TmxLayer> TmxTiledMap::LayerForName(const string& name) {
    for (auto layer : layers_) {
        if (layer->name() == name) {
            return layer;
        }
    }
    return nullptr;
}

shared_ptr<TmxTilesetInfo> TmxTiledMap::TilesetForLayer(const shared_ptr<TmxLayerInfo>& layer_info,
                                                        const shared_ptr<TmxMapInfo>& map_info) {
    ivec2 size_in_tiles = layer_info->size_in_tiles;
    for (auto iter = map_info->tilesets.rbegin();
         iter != map_info->tilesets.rend();
         ++iter) {
        auto tileset_info = *iter;

        for (size_t y = 0; y < size_in_tiles.y; ++y) {
            for (size_t x = 0; x < size_in_tiles.x; ++x) {
                size_t pos = x + size_in_tiles.x * y;
                tmx_gid gid = layer_info->gids[pos];

                gid = ntohl(gid);
                if (gid != 0) {
                    if ((gid & TmxTileFlags::FLIPPED) >= tileset_info->first_gid) {
                        return tileset_info;
                    }
                }
            }
        }
    }

    S2ERROR("TmxTiledMap: %s has no tiles", layer_info->name.c_str());
    return nullptr;
}

}
