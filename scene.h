#ifndef __sometuke__node_scene__
#define __sometuke__node_scene__

#include "sometuke/director.h"
#include "sometuke/handler.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/actor.h"
#include "sometuke/node/layer.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace sometuke {
using namespace std;

class Actor;

typedef unordered_map<ActorId, shared_ptr<Actor> > ActorTable;

class Scene : public Handler {
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

    void AddLayer(shared_ptr<Layer> layer) {
        root_node_->AddChild(layer);
    }

    void AddActor(const shared_ptr<Actor>& actor);
    void RemoveActor(const ActorId& id);
    
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

#endif /* defined(__sometuke__node_scene__) */
