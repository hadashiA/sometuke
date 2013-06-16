#ifndef __kawaii__node_layer__
#define __kawaii__node_layer__

#include "kawaii/node/actor.h"
#include "kawaii/logger.h"

#include <unordered_map>

namespace kawaii {
using namespace std;

typedef unordered_map<ActorId, weak_ptr<Actor>, ActorIdHash> ActorTable;

class Layer : public Node {
public:
    virtual ~Layer() {}

    bool Init() { return true; }

    void AddActor(shared_ptr<Actor> actor) {
        ActorTable::iterator i = actors_.find(actor->id());
        if (i == actors_.end()) {
            actors_[actor->id()] = actor;
            AddChild(actor);
        } else {
            IIWARN("Actor already exists id:%ld", actor->id().c_str());
        }
    }

    shared_ptr<Actor> FindActor(const ActorId& id) {
        weak_ptr<Actor> actor_weak = actors_[id];
        if (shared_ptr<Actor> actor = actor_weak.lock()) {
            return actor;
        } else {
            return NULL;
        }
    }

    shared_ptr<Actor> operator[](const ActorId& id) {
        return FindActor(id);
    }

protected:
    // virtual void AddChild(shared_ptr<Node> child) {
    //     Node::AddChild(child);
    // }

    ActorTable actors_;
};

}

#endif /* defined(__kawaii__node_layer__) */
