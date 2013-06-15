#ifndef __kawaii__node_node__
#define __kawaii__node_node__

#include "kawaii/types.h"
#include "kawaii/vector.h"
#include "kawaii/matrix.h"
#include "kawaii/color.h"
#include "kawaii/application.h"

#include <vector>
#include <memory>
#include <string>

namespace kawaii {
using namespace std;

class EventDispatcher;
class Texture2D;
class Sequence;

class LabelInterface {
public:
    virtual ~LabelInterface() {}
    virtual void set_text(const string& text) = 0;
    virtual const string& text() const = 0;
};

class TextureInterface {
public:
    virtual ~TextureInterface() {}
    virtual shared_ptr<Texture2D> texture() const = 0;
    virtual void set_texture(shared_ptr<Texture2D> value) = 0;
};

class RGBAInterface {
public:    
    virtual ~RGBAInterface() {}
    virtual const Color3B color() const = 0;
    virtual const GLubyte opacity() const = 0;
    virtual bool does_opacity_modify_rgb() const = 0;
    virtual void set_color(const Color3B& value) = 0;
    virtual void set_opacity(GLubyte value) = 0;
    virtual void set_opacity_modify_rgb(bool value) = 0;
};

class Node : public UpdateInterface,
             public enable_shared_from_this<Node> {
public:
    Node()
        : position_(0, 0, 0),
          scale_x_(1),
          scale_y_(1),
          rotation_(0),
          skew_x_(0),
          skew_y_(0),
          anchor_point_(0, 0),
          anchor_point_in_points_(0, 0),
          content_size_(0, 0),
          z_order_(0),
          paused_(false),
          is_visible_(true),
          is_transform_dirty_(true),
          is_inverse_dirty_(true) {
    }
    
    virtual ~Node() {}

    const vec3& position() const {
        return position_;
    }

    const float scale_x() const {
        return scale_x_;
    }

    const float scale_y() const {
        return scale_y_;
    }

    const float rotation() const {
        return rotation_;
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

    const vec2& anchor_point() const {
        return anchor_point_;
    }

    const vec2& anchor_point_in_points() const {
        return anchor_point_in_points_;
    }

    const vec2& content_size() const {
        return content_size_;
    }

    shared_ptr<Node> parent() {
        return parent_.lock();
    }

    void set_position(const vec3& position) {
        position_ = position;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void set_position(const float x, const float y, const float z = 0) {
        set_position(vec3(x, y, z));
    }

    void add_position(const vec3& diff) {
        position_ += diff;
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    void add_position(const float dx, const float dy, const float dz = 0) {
        add_position(vec3(dx, dy, dz));
    }

    void set_rotation(const float degrees) {
        rotation_ = degrees;
        is_transform_dirty_ = is_inverse_dirty_ = true;
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

    void set_anchor_point(const float x, const float y) {
        set_anchor_point(vec2(x, y));
    }

    void set_content_size(const vec2& value) {
        if (content_size_ != value) {
            content_size_ = value;
            anchor_point_in_points_ = vec2(content_size_.x * anchor_point_.x,
                                           content_size_.y * anchor_point_.y);
            is_transform_dirty_ = is_inverse_dirty_ = true;
        }
    }

    void set_content_size(const float x, const float y) {
        set_content_size(vec2(x, y));
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
    
    virtual EventDispatcher& dispatcher() const {
        return Application::Instance().director().dispatcher();
    }

    void ScheduleUpdate() {
        shared_ptr<UpdateInterface> self = static_pointer_cast<UpdateInterface>(shared_from_this());
        Application::Instance().director().scheduler().Schedule(self);
    }

    // Node& operator<<(shared_ptr<Node> child) { 
    //     AddChild(child);
    //     return *this;
    // }

    virtual const bool paused() {
        return paused_;
    }

    virtual bool Update(const ii_time delta_time) { return true; }   // Is not called by default

    virtual bool Init() { return true; }
    virtual void Render()  {}

    virtual void OnEnter() {
        for (vector<shared_ptr<Node> >::iterator i = children_.begin();
             i != children_.end(); ++i) {
            (*i)->OnEnter();
        }
        paused_ = false;
    }

    virtual void OnExit()  {
        for (vector<shared_ptr<Node> >::iterator i = children_.begin();
             i != children_.end(); ++i) {
            (*i)->OnExit();
        }
        paused_ = false;
    }

    void Visit();

    const mat4& LocalTransform();

    mat4 WorldTransform();

    vec3 WorldPosition();
    // void SetWorldPosition(const vec3& world_position);

    vec3 WorldPositionAt(const vec3& position);
    vec3 LocalPositionAt(const vec3& world_position);
    
    const Rect bounding_box() {
        Rect rect(0, 0, content_size_.x, content_size_.y);
        return rect * LocalTransform();
    }

    // process helpers
    virtual ProcessManager& process_manager() const {
        return Application::Instance().director().process_manager();
    }

    shared_ptr<Node> MoveTo(const ii_time duration, const vec3& to);
    shared_ptr<Node> MoveBy(const ii_time duration, const vec3& delta);
    shared_ptr<Node> Delay(const ii_time duration);
    shared_ptr<Node> Repeat(int repeat = -1);
    void Run();
    void Loop(int repeat = -1);
    void StopAllProcess();

protected:
    shared_ptr<Sequence> sequence();

    vec3 position_;
    float scale_x_;
    float scale_y_;
    float rotation_;
    float skew_x_;
    float skew_y_;
    vec2 anchor_point_;
    vec2 anchor_point_in_points_;
    vec2 content_size_;
    int z_order_;
    bool is_visible_;
    bool paused_;

    mat4 local_transform_;
    mat4 local_inverse_;
    bool is_transform_dirty_;
    bool is_inverse_dirty_;

    vector<shared_ptr<Node> > children_;
    weak_ptr<Node> parent_;

    shared_ptr<Sequence> sequence_;

};

}

#endif /* defined(__kawaii__node_node__) */
