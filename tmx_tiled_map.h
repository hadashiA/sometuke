#ifndef __sometuke__tmx_tiled_map__
#define __sometuke__tmx_tiled_map__

#include "sometuke/tmx_types.h"
#include "sometuke/vector.h"

#include <vector>

namespace sometuke {

class TmxTiledMap {
public:

private:
    ivec2 num_tiles_;
    vec2 tile_size;
    TmxOrientation orientation_;
    vector<TmxObjectGroup> object_groups_;
    vector<TmxProperties> tile_properties_;
};

}

#endif /* defined(__sometuke__tmx_tiled_map__) */
