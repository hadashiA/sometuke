#ifndef __kawaii__scene__
#define __kawaii__scene__

#include "actor.h"
#include "node.h"

#include <map>

namespace kawaii {
using namespace std;

class Scene : public Node {
public:
    Scene();
    virtual ~Scene();
    
    // void Visit() { Node::Visit(); }
    void AddChildFromActor(shared_ptr<Actor> actor);

protected:
    std::map<actor_id, shared_ptr<Node> > node_for_actor_id_;
};

}

#endif /* defined(__kawaii__scene__) */
