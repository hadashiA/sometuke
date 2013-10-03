#ifndef __sometuke__node_tmx_layer__
#define __sometuke__node_tmx_layer__

#include "sometuke/node/sprite_batch_node.h"
#include "sometuke/tmx_parser.h"

namespace sometuke {

class TmxLayer : public SpriteBatchNode {
public:
    bool InitWithTilesetInfo(const TmxTilesetInfo& tileset_info,
                             const TmxLayerInfo& layer_info,
                             const TmxMapInfo& map_info);

private:
    TmxTilesetInfo tileset_info_;
    char *name;
    ivec2 size_in_tiles_;
    TmxProperties properties_;
    tmx_gid gid_min_;
    tmx_gid gid_max_;
};
    
}

#endif /* defined(__Hitasura__tmx_layer__) */
