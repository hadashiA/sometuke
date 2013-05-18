#ifndef __kawaii__node_scene__
#define __kawaii__node_scene__

#include "kawaii/node/layer.h"
#include "kawaii/matrix_stack.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Scene : public Node {
public:
    virtual ~Scene() {}

    virtual bool Init() { return true; }
    
    virtual void Render() {}

    void AddLayer(shared_ptr<Layer> layer) {
        AddChild(layer);
    }

protected:
    virtual void AddChild(shared_ptr<Node> child) {
        Node::AddChild(child);
    }
};

}

#endif /* defined(__kawaii__node_scene__) */
