#ifndef __kawaii__node_layer__
#define __kawaii__node_layer__

#include "actor.h"
#include "logger.h"

#include <unordered_map>

namespace kawaii {
using namespace std;

typedef unordered_map<ActorId, weak_ptr<Actor> > ActorTable;

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
            IIWARN("Actor already exists id:%ld", actor->id());
        }
    }

    shared_ptr<Actor> FindActor(const ActorId id) {
        weak_ptr<Actor> actor_ref = actors_[id];
        if (shared_ptr<Actor> actor = actor_ref.lock()) {
            return actor;
        } else {
            return NULL;
        }
    }

    shared_ptr<Actor> operator[](const ActorId id) {
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
