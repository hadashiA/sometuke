#include "scene.h"

#include "process_manager.h"

namespace kawaii {

Scene::Scene()
    : process_manager_(new ProcessManager),
      root_node_(new Node),
      actor_table_(),
      node_for_actor_id_() {}

Scene::~Scene() {
}

void Scene::Update(const float delta_time) {
    process_manager_->Update(delta_time);
}

void Scene::Render() {
    root_node_->Visit();
}

void Scene::AddActor(const shared_ptr<Actor>& actor) {
    actor_id id = actor->id();
    actor_table_[id] = actor;
}

const shared_ptr<Actor> Scene::ActorById(const actor_id id) {
    return actor_table_[id];
}

}

