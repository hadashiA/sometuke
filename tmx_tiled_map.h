#ifndef __sometuke__tmx_tiled_map__
#define __sometuke__tmx_tiled_map__

#include "sometuke/tmx_types.h"
#include "sometuke/vector.h"

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

namespace sometuke {

namespace sometuke {
using namespace std;

class TmxTiledMap {
public:
    
private:
    vector<shared_ptr<TmxLayer> > layers_;
    vector<TmxObjectGroup> object_groups_;
    TmxProperties tile_properties_;
};
    
}

#endif /* defined(__sometuke__tmx_tiled_map__) */
