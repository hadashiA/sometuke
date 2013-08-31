#ifndef __Hitasura__collection__
#define __Hitasura__collection__

#include "sometuke/director.h"
#include "sometuke/handler.h"
#include "sometuke/actor.h"

#include <memory>
#include <unordered_map>

namespace sometuke {
using namespace std;

template <class A>
class ActorCollection : public Actor {
public:
    typedef unordered_map<ActorId, shared_ptr<A> > Map;

    ActorCollection()
        : Actor("collection") {
    }

    virtual ~ActorCollection() {}

    void Add(const shared_ptr<A>& actor) {
        typename Map::iterator i = map_.find(actor->id());
        if (i == map_.end()) {
            map_[actor->id()] = actor;
            Queue<ActorAddEvent>(actor);
            OnAdd(actor);
        } else {
            S2ERROR("already exists id:%s", actor->id().c_str());
        }
    }

    void Remove(const ActorId& actor_id) {
        typename Map::iterator i = map_.find(actor_id);
        if (i != map_.end()) {
            Queue<ActorRemoveEvent>(actor_id);
            OnRemove(i->second);
            map_.erase(i);
        }
    }

    void Remove(const shared_ptr<A>& actor) {
        Remove(actor->id());
    }

    virtual void OnAdd(const shared_ptr<A>& actor) {}
    virtual void OnRemove(const shared_ptr<A>& actor) {}
    
    shared_ptr<A> Find(const ActorId& actor_id) {
        return map_[actor_id];
    }

    shared_ptr<A> operator[](const ActorId& actor_id) {
        return map_[actor_id];
    }

private:
    Map map_;
};

}

#endif /* defined(__Hitasura__collection__) */
