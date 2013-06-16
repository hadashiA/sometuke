#ifndef __kawaii__node_scene__
#define __kawaii__node_scene__

#include "kawaii/matrix_stack.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Layer;
class Actor;

typedef unordered_map<ActorId, shared_ptr<Actor>, ActorIdHash > ActorTable;

class Scene {
public:
    Scene()
        : root_node_(new Node) {
    }

    virtual ~Scene() {}

    virtual bool Init() { return true; }
    
    virtual void Visit() {
        root_node_->Visit();
    }

    virtual void OnEnter() {
        root_node_->OnEnter();
    }

    virtual void OnExit() {
        root_node_->OnExit();
    }

    void AddLayer(shared_ptr<Layer> layer) {
        root_node_->AddChild(layer);
    }

    void AddActor(shared_ptr<Actor> actor) {
        ActorTable::iterator i = actors_.find(actor->id());
        if (i == actors_.end()) {
            actors_[actor->id()] = actor;
            AddChild(actor);
        } else {
            IIWARN("Actor already exists id:%ld", actor->id().c_str());
        }
    }

    void RemoveActor(const ActorId& id) {
        ActorTable::iterator i = actor_table_.find(id);
        if (i != actor_table_.end()) {
            actor_table_.erase();
        }
    }

    shared_ptr<Actor> FindActor(const ActorId& id) {
        return actor_table_[id];
    }

    shared_ptr<Actor> operator[](const ActorId& id) {
        return actor_table_[id];
    }

private:
    shared_ptr<Node> root_node_;
    ActorTable actor_table_;
};

}

#endif /* defined(__kawaii__node_scene__) */
