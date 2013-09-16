#ifndef __sometuke__node_tmx_tiled_map__
#define __sometuke__node_tmx_tiled_map__

#include "sometuke/tmx_parser.h"
#include "sometuke/node/node.h"
#include "sometuke/vector.h"

#include <vector>
#include <memory>
#include <unordered_map>

namespace sometuke {
using namespace std;

class TmxLayer;
class TmxObjectGroup;

class TmxTiledMap : public Node {
public:

private:
    vec2 map_size_;
    vec2 tile_size_;
    TmxOrientation orientation_;

    vector<shared_ptr<TmxObjectGroup> > object_groups_;
    unordered_map<string, string> properties_;
};

}

#endif /* defined(__sometuke__node_tmx_tiled_map__) */
