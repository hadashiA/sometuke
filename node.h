#ifndef __kawaii__node__
#define __kawaii__node__

#include "vector.h"
#include "actor.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class Node {
public:
    Node()
        : position_(0, 0, 0),
          scale_(1, 1),
          rotation_(0, 0),
          anchor_point_(0, 0),
          content_size_(0, 0),
          z_order_(0),
          is_visible_(true),
          is_running_(false) {
    }
    
    virtual ~Node() {}

    const vec3& position() const {
        return position_;
    }

    void set_position(const vec3& value) {
        position_ = value;
    }

    const vec2& scale() const {
        return scale_;
    }

    void set_scale(const vec2& value) {
        scale_ = value;
    }

    const int z_order() const {
        return z_order_;
    }

    void set_z_order(int value) {
        z_order_ = value;
    }

    const bool is_visible() const {
        return is_visible_;
    }

    void set_visible(bool value) {
        is_visible_ = value;
    }

    void show() {
        is_visible_ = true;
    }

    void hide() {
        is_visible_ = false;
    }

    const bool is_running() const {
        return is_running_;
    }

    void Visit();

    virtual void AddChild(shared_ptr<Node> child);

    Node& operator<<(shared_ptr<Node> child) {
        AddChild(child);
        return *this;
    }

    virtual void OnEnter() {}
    virtual void Render()  {}
    virtual void OnExit()  {}

private:    
    vec3 position_;
    vec2 scale_;
    vec2 rotation_;
    vec2 anchor_point_;
    vec2 content_size_;
    int z_order_;
    bool is_visible_;
    bool is_running_;

    vector<shared_ptr<Node> > children_;
};

}

#endif /* defined(__kawaii__node__) */
