#ifndef __kawaii__node_scene__
#define __kawaii__node_scene__

#include "kawaii/application.h"
#include "kawaii/matrix_stack.h"
#include "kawaii/actor.h"
#include "kawaii/node/layer.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Actor;

typedef unordered_map<ActorId, shared_ptr<Actor>, ActorIdHash> ActorTable;

class Scene : public enable_shared_from_this<Scene>,
              public UpdateInterface {
public:
    Scene()
        : root_node_(new Node) {
        root_node_->set_anchor_point(0.5, 0.5);
        root_node_->set_ignore_anchor_point_for_position(true);
        root_node_->set_content_size(Application::Instance().size_in_points());
    }

    virtual ~Scene() {
    }

    virtual bool Init() { return true; }
    
    void Visit() {
        root_node_->Visit();
        Render();
    }

    virtual void Render() {}

    virtual void OnEnter() {
        root_node_->OnEnter();
    }

    virtual void OnExit() {
        root_node_->OnExit();
    }

    virtual Scheduler& scheduler() const {
        return Application::Instance().director().scheduler();
    }

    virtual EventDispatcher& dispatcher() const {
        return Application::Instance().director().dispatcher();
    }

    void ScheduleUpdate() {
        shared_ptr<UpdateInterface> self = static_pointer_cast<UpdateInterface>(shared_from_this());
        scheduler().Schedule(self);
    }

    void ScheduleUpdate(const ii_time interval) {
        shared_ptr<UpdateInterface> self = static_pointer_cast<UpdateInterface>(shared_from_this());
        scheduler().Schedule(self, interval);
    }

    void AddLayer(shared_ptr<Layer> layer) {
        root_node_->AddChild(layer);
    }

    void AddActor(shared_ptr<Actor> actor) {
        ActorTable::iterator i = actor_table_.find(actor->id());
        if (i == actor_table_.end()) {
            actor_table_[actor->id()] = actor;
        } else {
            IIWARN("Actor already exists id:%s", actor->id().c_str());
        }
    }

    void RemoveActor(const ActorId& id) {
        ActorTable::iterator i = actor_table_.find(id);
        if (i != actor_table_.end()) {
            actor_table_.erase(i);
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
