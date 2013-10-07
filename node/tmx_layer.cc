#include "sometuke/node/tmx_layer.h"

#include "sometuke/logger.h"

#include <arpa/inet.h>

namespace sometuke {

bool TmxLayer::InitWithTilesetInfo(const shared_ptr<TmxTilesetInfo>& tileset_info,
                                   const shared_ptr<TmxLayerInfo>& layer_info,
                                   const shared_ptr<TmxMapInfo>& map_info) {
    Director& director = Director::Instance();
    
    const shared_ptr<Texture2D>& texture =
        TextureCache::Instance().FetchFromPath(tileset_info.source_image);
    size_t num_tiles = layer_info.size_in_tiles.x * layer_info.size_in_tiles.y;
    size_t capacity  = ceil(num_tiles * / 2);

    if (!InitWithTexture(texture, capacity)) {
        S2ERROR("TmxLayer: faild texture init");
        return false;
    }

    name_          = layer_info->name;
    size_in_tiles_ = layer_info->size_in_tiles;
    gids_          = layer_info->gids;
    gid_min_       = layer_info->gid_min;
    gid_max_       = layer_info->gid_max;
    opacity_       = layer_info->opacity;
    properties_    = layer_info->properties;

    vec2 offset = CalculateLayerOffset(layer_info->offset);
    set_position(offset * director.content_scale_factor);

    vec2 size_in_pixels(layer_info->size_in_tiles.x * map_info->tile_size.x,
                        layer_info->size_in_tiles.y * map_info->tile_size.y);
    set_content_size(size_in_pixels / director.content_scale_factor);

    return true;
}

vec2 TmxLayer::CalculateLayerOffset(const vec2& pos) {
    if (orientation_ == TmxOrientation::ORTHO) {
        retrun vec2(pos.x * tile_size_.x, -pos.y * tile_size_.y);
    } else {
        S2ERROR("TmxLayer: orientation not suppored %d", orientation_);
    }
    return vec2(0, 0);
}

void TmxLayer::SetupTiles() {
    texture_atlas()->texture()->SetAliasTexParameters();

    for (size_t y = 0; y < size_in_tiles_.y; y++) {
        for (size_t x = 0; x < size_in_tiles_.x; x++) {
            size_t pos = x + size_in_tiles_.x * y;
            tmx_gid gid = gids[pos];

            // convert from big endian
            gid = ntohl(gid);

            if (gid != 0) {
                AppendTileForGid(gid, vec2(x, y));
                gid_min_ = std::min(gid_min_, gid);
                gid_max_ = std::max(gid_max_, gid);
            }            
        }
    }
}

Rect RectForGid(tmx_gid gid) {
    Rect rect;
    
    rect.size = tileset_info_->tile_size;
    return rect;
}

void TmxLayer::AppendTileForGid(tmx_gid gid, const vec2& tile_coord) {
    
}

}
