#ifndef __kawaii__scene__
#define __kawaii__scene__

#include "actor.h"
#include "application.h"
#include "matrix_stack.h"

#include <memory>
#include <unordered_map>

namespace kawaii {
using namespace std;

typedef unordered_map<actor_id, shared_ptr<Actor> > ActorTable;

class Scene : public Node {
public:
    virtual ~Scene() {}

    virtual bool HandleEvent(shared_ptr<Event> event) { return true; }

    virtual void Render() {
        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // MatrixStack<GLModelView>::Instance().Push();

        // Visit();

        // MatrixStack<GLModelView>::Instance().Pop();
    }

    void AddActor(shared_ptr<Actor> actor) {
        actors_[actor->id()] = actor;
        AddChild(actor);
    }

    shared_ptr<Actor> GetActor(const actor_id id) {
        return actors_[id];
    }

    shared_ptr<Actor> operator[](const actor_id id) {
        return actors_[id];
    }

protected:
    ActorTable actors_;
};

}

#endif /* defined(__kawaii__scene__) */
