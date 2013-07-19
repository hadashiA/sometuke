#include "skidarake/node/layer.h"

#include "skidarake/application.h"

namespace skidarake {

Layer::Layer() {
    set_anchor_point(0.5, 0.5);
    set_ignore_anchor_point_for_position(true);
    set_content_size(Application::Instance().size_in_points());
}

shared_ptr<Node> Layer::FindNodeByActorId(const ActorId& id) {
    ActorNodeTable::iterator i = actor_node_table_.find(id);
    if (i == actor_node_table_.end()) {
        return NULL;
    } else {
        weak_ptr<Node> node_weak = i->second;
        if (shared_ptr<Node> node = node_weak.lock()) {
            return node;
        } else {
            actor_node_table_.erase(i);
            return NULL;
        }
    }
}

}
