#ifndef __sometuke__node_scene__
#define __sometuke__node_scene__

#include "sometuke/actor_collection.h"
#include "sometuke/actor.h"
#include "sometuke/node/layer.h"

namespace sometuke {
using namespace std;

class Scene : public ActorCollection<Actor> {
public:
    Scene()
        : root_node_(new Node) {
        root_node_->set_anchor_point(0.5, 0.5);
        root_node_->set_ignore_anchor_point_for_position(true);
        root_node_->set_content_size(Director::Instance().size_in_points());
    }

    virtual ~Scene() {}

    virtual bool Init() { return true; }

    virtual void Render() {}

    virtual void OnEnter()   {}
    virtual void OnExit()    {}
    virtual void OnCleanup() {}

    void OnAdd(const shared_ptr<Actor>& actor) {
        Queue<ActorAddEvent>(actor);
    }

    void OnRemove(const shared_ptr<Actor>& actor) {
        Queue<ActorRemoveEvent>(actor->id());
    }

    void Visit() {
        root_node_->Visit();
        Render();
    }

    void Enter() {
        root_node_->Enter();
        WakeUp();
        OnEnter();
    }

    void Exit() {
        root_node_->Exit();
        Sleep();
        OnExit();
    }

    void AddLayer(const shared_ptr<Layer>& layer) {
        root_node_->AddChild(layer);
    }

private:
    shared_ptr<Node> root_node_;
};

}

#endif /* defined(__sometuke__node_scene__) */
