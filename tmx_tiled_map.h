#ifndef __sometuke__tmx_tiled_map__
#define __sometuke__tmx_tiled_map__

#include "sometuke/tmx_types.h"
#include "sometuke/vector.h"

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

namespace sometuke {
using namespace std;
    
class TmxLayer;

class TmxTiledMap {
public:
    bool InitWithTmxFile(const string& file);

private:
    shared_ptr<TmxTilesetInfo> TilesetForLayer(const shared_ptr<TmxLayerInfo>& layer_info,
                                               const shared_ptr<TmxMapInfo>& map_info);

    TmxOrientation orientation_;
    ivec2 size_in_tiles_;
    vec2 tile_size_;
    vector<shared_ptr<TmxLayer> > layers_;
    vector<shared_ptr<TmxObjectGroup> > object_groups_;
    TmxProperties properties_;
};
    
}

#endif /* defined(__sometuke__tmx_tiled_map__) */
