#ifndef __kawaii__scene__
#define __kawaii__scene__

#include "types.h"
#include "actor.h"
#include "node.h"
#include "event_dispatcher.h"

#include <memory>
#include <map>

namespace kawaii {
using namespace std;

class Actor;
    
typedef map<actor_id, shared_ptr<Actor> > ActorTable;

class Scene : public EventListener {
public:
    Scene()
        : root_node_(new Node),
          actor_table_(),
          node_for_actor_id_() {
    }

    virtual ~Scene() {}

    const shared_ptr<Node> root_node() const {
        return root_node_;
    }

    virtual bool Init() = 0;
    virtual bool HandleEvent(const Event& event) = 0;

    void Render() {
        root_node_->Visit();
    }

    void AddActor(shared_ptr<Actor> actor) {
        actor_id id = actor->id();
        actor_table_[id] = actor;
    }

    const shared_ptr<Actor> ActorById(const actor_id id) {
        return actor_table_[id];
    }

    void AddChildFromActor(shared_ptr<Actor> actor);

protected:
    ActorTable actor_table_;
    std::map<actor_id, shared_ptr<Node> > node_for_actor_id_;

    shared_ptr<Node> root_node_;
};

}

#endif /* defined(__kawaii__scene__) */
