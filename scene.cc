#include "scene.h"

#include "matrix_stack.h"
#include "actor.h"

#include <OpenGLES/ES2/gl.h>

namespace kawaii {

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

