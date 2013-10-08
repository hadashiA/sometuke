#ifndef __sometuke__node_tmx_layer__
#define __sometuke__node_tmx_layer__

#include "sometuke/node/sprite_batch_node.h"
#include "sometuke/tmx_types.h"

namespace sometuke {

class TmxLayer : public SpriteBatchNode {
public:
    TmxLayer()
        : orientation_(TmxOrientation::ORTHO) {
    }

    bool InitWithTilesetInfo(const shared_ptr<TmxTilesetInfo>& tileset_info,
                             const shared_ptr<TmxLayerInfo>& layer_info,
                             const shared_ptr<TmxMapInfo>& map_info);
    
    const string name() const {
        return name_;
    }

private:
    vec2 CalculateLayerOffset(const vec2& pos);
    void SetupTiles();
    Rect RectForGid(tmx_gid gid);
    void AppendTileForGid(tmx_gid gid, const vec2& tile_coord);

    string name_;
    ivec2 size_in_tiles_;
    TmxProperties properties_;
    tmx_gid gid_min_;
    tmx_gid gid_max_;
    vector<tmx_gid> gids_;
    unsigned char opacity_;

    vec2 tile_size_;

    shared_ptr<TmxTilesetInfo> tileset_info_;
    TmxOrientation orientation_;
};
    
}

#endif /* defined(__Hitasura__tmx_layer__) */
