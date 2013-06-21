#include "kawaii/node/layer.h"

#include "kawaii/application.h"
#include "kawaii/events.h"

namespace kawaii {

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

bool Layer::InitEventListen() {
    EventDispatcher& dispatcher = Application::Instance().director().dispatcher();

    shared_ptr<EventListener> listener = dispatcher.CreateListener<Layer>(this);
    dispatcher.On<MoveActorEvent>(listener);
    dispatcher.On<RotateActorEvent>(listener);
    
    return true;
}


bool Layer::HandleEvent(shared_ptr<Event> event) {
    if (event->type == MoveActorEvent::TYPE) {
        shared_ptr<MoveActorEvent> move = static_pointer_cast<MoveActorEvent>(event);

        if (shared_ptr<Node> node = FindNodeByActorId(move->actor_id)) {
            node->set_position(move->position);
        }
        
    } else if (event->type == RotateActorEvent::TYPE) {
        shared_ptr<RotateActorEvent> rotate = static_pointer_cast<RotateActorEvent>(event);
        if (shared_ptr<Node> node = FindNodeByActorId(rotate->actor_id)) {
            node->set_rotation(rotate->rotate);
        }
    }

    return true;
}

}
