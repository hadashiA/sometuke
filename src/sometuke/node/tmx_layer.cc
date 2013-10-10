#include "sometuke/node/tmx_layer.h"

#include "sometuke/texture_cache.h"
#include "sometuke/texture_2d.h"
#include "sometuke/logger.h"
#include "sometuke/node/sprite.h"

#include <arpa/inet.h>

namespace sometuke {

bool TmxLayer::InitWithTilesetInfo(const shared_ptr<TmxTilesetInfo>& tileset_info,
                                   const shared_ptr<TmxLayerInfo>& layer_info,
                                   const shared_ptr<TmxMapInfo>& map_info) {
    Director& director = Director::Instance();
    
    const shared_ptr<Texture2D>& texture =
        TextureCache::Instance().FetchFromPath(tileset_info->image_source);
    size_t num_tiles = layer_info->size_in_tiles.x * layer_info->size_in_tiles.y;
    size_t capacity  = ceil(num_tiles * 0.5);

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

    vec2 offset = CalculateLayerOffset(layer_info->offset_in_tiles);
    set_position(offset * director.content_scale_factor());

    vec2 size_in_pixels(layer_info->size_in_tiles.x * map_info->tile_size.x,
                        layer_info->size_in_tiles.y * map_info->tile_size.y);
    set_content_size(size_in_pixels / director.content_scale_factor());

    texture_atlas()->texture()->SetAliasTexParameters();

    // setup tiles
    for (size_t y = 0; y < size_in_tiles_.y; y++) {
        for (size_t x = 0; x < size_in_tiles_.x; x++) {
            size_t pos = x + size_in_tiles_.x * y;
            tmx_gid gid = gids_[pos];

            // convert from big endian
            gid = ntohl(gid);

            if (gid != 0) {
                AppendTileForGid(gid, vec2(x, y));
                gid_min_ = std::min(gid_min_, gid);
                gid_max_ = std::max(gid_max_, gid);
            }            
        }
    }

    return true;
}

vec3 TmxLayer::PositionAt(const vec2& tile_coord) {
    if (orientation_ == TmxOrientation::ORTHO) {
        return vec2(tile_coord.x * tile_size_.x,
                    (size_in_tiles_.y - tile_coord.y - 1) * tile_size_.y,
                    0);
    }
    return vec2(0, 0);
}

vec2 TmxLayer::CalculateLayerOffset(const vec2& pos) {
    if (orientation_ == TmxOrientation::ORTHO) {
        return vec2(pos.x * tile_size_.x, -pos.y * tile_size_.y);
    } else {
        S2ERROR("TmxLayer: orientation not suppored %d", orientation_);
    }
    return vec2(0, 0);
}

Rect TmxLayer::RectForGid(tmx_gid gid) {
    Rect rect;
    
    rect.size = tileset_info_->tile_size;

    tmx_gid local_id = (gid & TmxTileFlags::FLIPPED) - tileset_info_->first_gid;

    const vec2& image_size = tileset_info_->image_size;
    const vec2& tile_size  = tileset_info_->tile_size;
    float margin  = tileset_info_->margin;
    float spacing = tileset_info_->spacing;

    int max_x = (image_size.x - margin * 2 + spacing) / (tile_size.x + spacing);
    rect.pos.x = (local_id % max_x) * (tile_size.x + spacing) + margin;
    rect.pos.y = (local_id / max_x) * (tile_size.y + spacing) + margin;

    return rect;
}

void TmxLayer::AppendTileForGid(tmx_gid gid, const vec2& tile_coord) {
    unsigned int z = tile_coord.x + tile_coord.y * size_in_tiles_.x;
}

void TmxLayer::SetupTileAt(const vec2& tile_coord, tmx_gid gid) {
    Rect rect = RectForGid(gid) * Director::Instance().content_scale_factor();

    if (!reused_sprite_) {
        reused_sprite_ = make_shared<Sprite>();
        auto batch_node = static_pointer_cast<SpriteBatchNode>(shared_from_this());
        reused_sprite_->InitWithTexture(texture_atlas()->texture(), rect, false);
        reused_sprite_->set_batch_node(batch_node, 0);
    } else {
        reused_sprite_->reset_batch_node();
        reused_sprite_->set_texture_rect(rect, false, rect.size);
    }

    reused_sprite_->set_position(PositionAt(tile_coord));
    reused_sprite_->set_opacity(opacity_);
    reused_sprite_->flip_x(false);
    reused_sprite_->flip_y(false);
    reused_sprite_->set_rotation(0);
    reused_sprite_->set_anchor_point(0, 0);

    if (gid & TmxTileFlags::DIAGONAL) {
        reused_sprite_->set_anchor_point(0.5, 0.5);
        vec2 pos  = reused_sprite_->position();
        vec2 size = reused_sprite_->content_size();
        reused_sprite_->set_position(pos.x + size.y / 2, pos.y + size.x / 2);

        uint32_t flag = gid & (TmxTileFlags::HORIZONTAL | TmxTileFlags::VERTICAL);
        if (flag == TmxTileFlags::HORIZONTAL) {
            reused_sprite_->set_rotation(90);
        } else if (flag == TmxTileFlags::VERTICAL) {
            reused_sprite_->set_rotation(270);
        } else if (flag == (TmxTileFlags::HORIZONTAL | TmxTileFlags::VERTICAL)) {
            reused_sprite_->set_rotation(90);
            reused_sprite_->flip_x(true);
        } else {
            reused_sprite_->set_rotation(270);
            reused_sprite_->flip_x(true);
        }

    } else {
        if (gid & TmxTileFlags::HORIZONTAL) {
            reused_sprite_->flip_x(true);
        }
        if (gid & TmxTileFlags::VERTICAL) {
            reused_sprite_->flip_y(true);
        }
    }
}

}
