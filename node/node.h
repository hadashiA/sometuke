#ifndef __kawaii__node_node__
#define __kawaii__node_node__

#include "types.h"
#include "vector.h"
#include "matrix.h"
#include "kawaii/process/node_process_relation.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class ProcessScheduler;
class EventDispatcher;

class Node : public enable_shared_from_this<Node> {
public:
    Node()
        : local_position_(0, 0, 0),
          scale_x_(1),
          scale_y_(1),
          rotation_x_(0),
          rotation_y_(0),
          skew_x_(0),
          skew_y_(0),
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

    const float scale_x() const {
        return scale_x_;
    }

    const float scale_y() const {
        return scale_y_;
    }

    const float rotation_x() const {
        return rotation_x_;
    }

    const float rotation_y() const {
        return rotation_y_;
    }

    const float skew_x() const {
        return skew_x_;
    }

    const float skew_y() const {
        return skew_y_;
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

    const vec2& content_size() const {
        return content_size_;
    }

    shared_ptr<Node> parent() {
        return parent_.lock();
    }

    void set_local_position(const vec3& position) {
        local_position_ = position;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_local_position(const float x, const float y, const float z = 0) {
        set_local_position(vec3(x, y, z));
    }

    void add_position(const vec3& diff) {
        local_position_ += diff;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void add_position(const float dx, const float dy, const float dz = 0) {
        add_position(vec3(dx, dy, dz));
    }

    void set_scale_x(const float scale_x) {
        scale_x_ = scale_x;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_scale_y(const float scale_y) {
        scale_y_ = scale_y;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_scale(const float scale) {
        scale_x_ = scale;
        scale_y_ = scale;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_skew_x(const float skew_x) {
        skew_x_ = skew_x;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_skew_y(const float skew_y) {
        skew_y_ = skew_y;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_z_order(int z_order) {
        z_order = z_order;
        // parent_->ReorderChild(this, z_order)
    }

    void set_anchor_point(const vec2& value) {
        if (anchor_point_ != value) {
            anchor_point_ = value;
            anchor_point_in_points_ = vec2(content_size_.x * anchor_point_.x,
                                           content_size_.y * anchor_point_.y);
            is_transform_dirty_ = is_inverse_dirty_ = true;
        }
    }

    void set_content_size(const vec2& value) {
        if (content_size_ != value) {
            content_size_ = value;
            anchor_point_in_points_ = vec2(content_size_.x * anchor_point_.x,
                                           content_size_.y * anchor_point_.y);
            is_transform_dirty_ = is_inverse_dirty_ = true;
        }
    }

    void show() {
        is_visible_ = true;
    }

    void hide() {
        is_visible_ = false;
    }

    void set_parent(shared_ptr<Node> value) {
        parent_ = value;
    }

    bool anchor_point_is_zero() const {
        return (anchor_point_.x == 0 && anchor_point_.y == 0);
    }

    vector<shared_ptr<Node> > children() const {
        return children_;
    }

    virtual void AddChild(shared_ptr<Node> child);

    virtual ProcessScheduler& scheduler() const;
    virtual EventDispatcher& dispatcher() const;

    // Node& operator<<(shared_ptr<Node> child) { 
    //     AddChild(child);
    //     return *this;
    // }

    virtual bool Init() { return true; }
    virtual void Update(const ii_time delta_time) {}   // Is not called by default
    virtual void OnEnter() {}
    virtual void Render()  {}
    virtual void OnExit()  {}

    virtual NodeProcessRelation Does() {
        return NodeProcessRelation(shared_from_this());
    }

    void Visit();
    void ScheduleUpdate();

    const mat4& LocalTransform();

    mat4 WorldTransform();

    vec3 WorldPosition();
    // void SetWorldPosition(const vec3& world_position);

    vec3 WorldPositionAt(const vec3& local_position);
    vec3 LocalPositionAt(const vec3& world_position);

protected:    
    vec3 local_position_;
    float scale_x_;
    float scale_y_;
    float rotation_x_;
    float rotation_y_;
    float skew_x_;
    float skew_y_;
    vec2 anchor_point_;
    vec2 anchor_point_in_points_;
    vec2 content_size_;
    int z_order_;
    bool is_visible_;
    bool is_running_;

    mat4 local_transform_;
    mat4 local_inverse_;
    bool is_transform_dirty_;
    bool is_inverse_dirty_;

    vector<shared_ptr<Node> > children_;
    weak_ptr<Node> parent_;
};

}

#endif /* defined(__kawaii__node_node__) */
