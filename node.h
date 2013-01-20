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
        : local_position_(0, 0, 0),
          scale_(1, 1),
          rotation_(0),
          skew_(0, 0),
          anchor_point_(0, 0),
          content_size_(0, 0),
          z_order_(0),
          is_visible_(true),
          is_running_(false),
          is_transform_dirty_(true),
          is_inverse_dirty_(true) {
    }
    
    virtual ~Node() {}

    const vec3& local_position() const {
        return local_position_;
    }

    const vec2& scale() const {
        return scale_;
    }

    const float rotation() const {
        return rotation_;
    }

    const vec2& skew() const {
        return skew_;
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

    const vec2& anchor_point() const {
        return anchor_point_;
    }

    void set_local_position(const vec3& local_position);
    void add_position(const vec3& diff);
    void set_scale_x(const float scale_x);
    void set_scale_y(const float scale_y);
    void set_scale(const float scale);
    void set_skew_x(const float skew_x);
    void set_skew_y(const float skew_y);
    void set_rotation(const float rotation);
    void set_z_order(int z_order);
    void set_anchor_point(const vec2& point);

    void show() {
        is_visible_ = true;
    }

    void hide() {
        is_visible_ = false;
    }

    bool AnchorPointIsZero() const {
        return (anchor_point_.x == 0 && anchor_point_.y == 0);
    }

    const vec3& WorldPosition() const;
    void SetWorldPosition(const vec3& world_position);

    virtual void AddChild(shared_ptr<Node> child);

    // Node& operator<<(shared_ptr<Node> child) { 
    //     AddChild(child);
    //     return *this;
    // }

    virtual void Update(const ii_time delta_time) {}   // Is not called by default
    virtual void OnEnter() {}
    virtual void Render()  {}
    virtual void OnExit()  {}

    void Visit();
    void ScheduleUpdate();

    const mat4& NodeToParentTransform();
    const mat4& ParentToNodeTransform();

private:    
    vec3 local_position_;
    vec2 scale_;
    float rotation_;
    vec2 skew_;
    vec2 anchor_point_;
    vec2 content_size_;
    int z_order_;
    bool is_visible_;
    bool is_running_;

    mat4 transform_;
    mat4 inverse_;
    bool is_transform_dirty_;
    bool is_inverse_dirty_;

    vector<shared_ptr<Node> > children_;
};

}

#endif /* defined(__kawaii__node__) */
