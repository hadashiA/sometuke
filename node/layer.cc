#include "kawaii/node/layer.h"

namespace kawaii {

shared_ptr<Actor> Layer::FindNodeByActorId(const ActorId& id) {
    ActorNodeTable::iterator i = actor_node_table_.find(id);
    if (i == actor_node_table_.end()) {
        return NULL;
    } else {
        weak_ptr<Node> node_weak = (*i);
        if (shared_ptr<Node> node = node_weak.lock()) {
            return node;
        } else {
            actor_node_table_.erase(i);
            return NULL;
        }
    }
}

}
