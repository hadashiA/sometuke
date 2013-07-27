#ifndef __sometuke__node_scene__
#define __sometuke__node_scene__

#include "sometuke/director.h"
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

class Scene : public enable_shared_from_this<Scene> {
public:
    Scene()
        : paused_(false),
          root_node_(new Node) {
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

    virtual void HandleEvent(const shared_ptr<Event>& event) = 0;
    virtual bool Update(const s2_time delta) { return true; }

    virtual const bool paused() const { return paused_; }
    virtual void Pause()  { paused_ = true; }
    virtual void Resume() { paused_ = false; }

    void Visit() {
        root_node_->Visit();
        Render();
    }

    void Enter() {
        root_node_->Enter();
        paused_ = false;
        OnEnter();
    }

    void Exit() {
        root_node_->Exit();
        paused_ = true;
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

protected:
    EventListener& listener() {
        if (!listener_) {
            listener_ = EventAdapter<Scene>::Create(this);
        }
        return *listener_;
    }

    Timer& timer() {
        if (!timer_) {
            timer_ = TimerAdapter<Scene>::Create(this);
        }
        return *timer_;
    }

private:
    shared_ptr<Node> root_node_;
    ActorTable actor_table_;
    shared_ptr<EventListener> listener_;
    shared_ptr<Timer> timer_;
    bool paused_;
};

}

#endif /* defined(__sometuke__node_scene__) */
