#ifndef __kawaii__scene__
#define __kawaii__scene__

#include "node.h"
#include "event_dispatcher.h"
#include "matrix_stack.h"

#include <memory>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Scene : public enable_shared_from_this<Scene> {
public:
    Scene()
        : root_node_(new Node),
          node_for_actor_id_() {
    }

    virtual ~Scene() {}

    virtual bool Init() { return true; }
    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual bool HandleEvent(shared_ptr<Event> event) { return true; }

    void Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        MatrixStack::GLModelView()->Push();

        root_node_->Visit();

        MatrixStack::GLModelView()->Pop();
    }

    void AddChild(actor_id id, shared_ptr<Node> node) {
        node_for_actor_id_[id] = node;
        root_node_->AddChild(node);
    }

    shared_ptr<Node> ActorForId(const actor_id aid) {
        return node_for_actor_id_[aid];
    }

protected:
    shared_ptr<Node> root_node_;
    unordered_map<actor_id, shared_ptr<Node> > node_for_actor_id_;
};

}

#endif /* defined(__kawaii__scene__) */
