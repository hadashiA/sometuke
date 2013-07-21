#ifndef __sometuke__node_scene__
#define __sometuke__node_scene__

#include "sometuke/application.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/actor.h"
#include "sometuke/node/layer.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace sometuke {
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

    virtual ~Scene() {}

    virtual bool Init() { return true; }

    virtual void Render() {}

    virtual void OnEnter()   {}
    virtual void OnExit()    {}
    virtual void OnCleanup() {}

    virtual bool HandleEvent(shared_ptr<Event> event) = 0;
    virtual bool Update(const ii_time delta) { return true; }

    const shared_ptr<EventListener>& listener() {
        if (!listener_) {
            listener_ = EventDelegator<Scene>::Create(this);
        }
        return listener_;
    }

    const shared_ptr<Timer>& timer() {
        if (!timer_) {
            timer_ = TimerDelegate<Scene>::Create(this);
        }
        return timer_;
    }

    void Visit() {
        root_node_->Visit();
        Render();
    }

    void Enter() {
        root_node_->Enter();
        if (listener_) {
            listener_->Resume();
        }
        if (timer_) {
            timer_->Resume();
        }
        OnEnter();
    }

    void Exit() {
        root_node_->Exit();
        if (listener_) {
            listener_->Pause();
        }
        if (timer_) {
            timer_->Pause();
        }
        OnExit();
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
    shared_ptr<EventListener> listener_;
    shared_ptr<Timer> timer_;
};

}

#endif /* defined(__sometuke__node_scene__) */
