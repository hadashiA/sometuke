#include "scene.h"

namespace kawaii {

Scene::Scene()
    : root_node_(new Node),
      actor_table_(),
      node_for_actor_id_() {}

Scene::~Scene() {
}

void Scene::Render() {
    root_node_->Visit();
}

void Scene::AddActor(shared_ptr<Actor> actor) {
    actor_id id = actor->id();
    actor_table_[id] = actor;
}

const shared_ptr<Actor> Scene::ActorById(const actor_id id) {
    return actor_table_[id];
}

}

