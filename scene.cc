#include "scene.h"

#include "matrix_stack.h"
#include "actor.h"

#include <OpenGLES/ES2/gl.h>

namespace kawaii {

Scene::Scene()
    : root_node_(new Node),
      actor_table_(),
      node_for_actor_id_() {}

Scene::~Scene() {
}

void Scene::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack::GLModelView()->Push();

    root_node_->Visit();

    MatrixStack::GLModelView()->Pop();
}

void Scene::AddActor(shared_ptr<Actor> actor) {
    actor_id id = actor->id();
    actor_table_[id] = actor;
}

const shared_ptr<Actor> Scene::ActorById(const actor_id id) {
    return actor_table_[id];
}

}

