#include "scene.h"

#include "director.h"

namespace sometuke {

void Scene::AddActor(const shared_ptr<Actor>& actor) {
    ActorTable::iterator i = actor_table_.find(actor->id());
    if (i == actor_table_.end()) {
        actor_table_[actor->id()] = actor;
        Director::Instance().dispatcher().Queue<ActorAddEvent>(actor->id(), actor->type());

    } else {
        IIWARN("Actor already exists id:%s", actor->id().c_str());
    }
}

void Scene::RemoveActor(const ActorId& id) {
    ActorTable::iterator i = actor_table_.find(id);
    if (i != actor_table_.end()) {
        actor_table_.erase(i);
        Director::Instance().dispatcher().Queue<ActorRemoveEvent>(id);
    }
}

}
