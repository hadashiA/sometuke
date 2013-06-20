#ifndef __kawaii__node_layer__
#define __kawaii__node_layer__

#include "kawaii/actor_id.h"
#include "kawaii/logger.h"
#include "kawaii/node/node.h"

#include <unordered_map>

namespace kawaii {
using namespace std;

typedef unordered_map<ActorId, weak_ptr<Node>, ActorIdHash> ActorNodeTable;

class Layer : public Node {
public:
    Layer();

    virtual ~Layer() {
    }

    bool Init() { return true; }

    virtual void AddChild(shared_ptr<Node> child) {
        Node::AddChild(child);

        const ActorId& actor_id = child->actor_id();
        if (!actor_id.is_null()) {
            //ActorNodeTable::iterator i = actor_node_table_.find(actor_id);
            actor_node_table_[actor_id] = child;
        }
    }

    shared_ptr<Node> FindNodeByActorId(const ActorId& id);
    
    shared_ptr<Node> operator[](const ActorId& id) {
        return FindNodeByActorId(id);
    }

protected:
    ActorNodeTable actor_node_table_;
};

}

#endif /* defined(__kawaii__node_layer__) */
