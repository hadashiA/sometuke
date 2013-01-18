#ifndef __kawaii__node__
#define __kawaii__node__

#include "types.h"
#include "vector.h"
#include "matrix.h"

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
          is_running_(false),
          is_transform_dirty_(true) {
    }
    
    virtual ~Node() {}

    const vec3& position() const {
        return position_;
    }

    const vec2& scale() const {
        return scale_;
    }

    const int z_order() const {
        return z_order_;
    }

    const bool is_visible() const {
        return is_visible_;
    }

    const bool is_running() const {
        return is_running_;
    }

    void set_position(const vec3& position);
    void add_position(const vec3& diff);
    void set_scale_x(const float scale_x);
    void set_scale_y(const float scale_y);
    void set_scale(const float scale);
    void set_z_order(int z_order);

    void show() {
        is_visible_ = true;
    }

    void hide() {
        is_visible_ = false;
    }

    virtual void AddChild(shared_ptr<Node> child);

    Node& operator<<(shared_ptr<Node> child) {
        AddChild(child);
        return *this;
    }

    void Visit();
    void ScheduleUpdate();

    virtual void Update(const ii_time delta_time) {}   // Is not called by default
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

    bool is_transform_dirty_;

    vector<shared_ptr<Node> > children_;
};

}

#endif /* defined(__kawaii__node__) */
