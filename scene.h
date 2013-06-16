#ifndef __kawaii__node_scene__
#define __kawaii__node_scene__

#include "kawaii/node/layer.h"
#include "kawaii/matrix_stack.h"

#include <memory>
#include <vector>
#include <unordered_map>

typedef unordered_map<ActorId, shared_ptr<Actor>> ActorTable;

namespace kawaii {
using namespace std;

class Scene {
public:
    Scene()
        : root_node_(new Node) {
    }

    virtual ~Scene() {}

    virtual bool Init() { return true; }
    
    virtual void Render() {}

    void AddLayer(shared_ptr<Layer> layer) {
        root_node_->AddChild(layer);
    }

private:
    shared_ptr<Node> root_node_;
    ActorTable actor_table_;
};

}

#endif /* defined(__kawaii__node_scene__) */
