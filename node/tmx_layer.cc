#include "sometuke/node/tmx_layer.h"

#include "sometuke/logger.h"

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

    set_content_size();

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

}
