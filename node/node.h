#ifndef __sometuke__node_node__
#define __sometuke__node_node__

#include "sometuke/handler.h"
#include "sometuke/types.h"
#include "sometuke/actor_id.h"
#include "sometuke/vector.h"
#include "sometuke/matrix.h"
#include "sometuke/color.h"

#include <vector>
#include <memory>
#include <string>

namespace sometuke {
using namespace std;

class EventDispatcher;
class Texture2D;

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
    virtual void set_texture(const shared_ptr<Texture2D>& value) = 0;
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

class Node : public Handler {
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
          ignore_anchor_point_for_position_(false),
          content_size_(0, 0),
          z_order_(0),
          visible_(true),
          is_transform_dirty_(true),
          is_inverse_dirty_(true),
          is_reorder_child_dirty_(true) {
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

    const bool visible() const {
        return visible_;
    }

    const vec2& anchor_point() const {
        return anchor_point_;
    }

    const vec2& anchor_point_in_points() const {
        return anchor_point_in_points_;
    }

    const bool ignore_anchor_point_for_position() const {
        return ignore_anchor_point_for_position_;
    }

    const vec2& content_size() const {
        return content_size_;
    }

    const ActorId& actor_id() const {
        return actor_id_;
    }

    shared_ptr<Node> parent() {
        return parent_.lock();
    }

    void set_position(const vec3& position) {
        position_ = position;
        OnChangeTransform();
    }

    void set_position(const float x, const float y, const float z = 0) {
        set_position(vec3(x, y, z));
    }

    void set_position(const vec2& posv2, const float z) {
        set_position(vec3(posv2.x, posv2.y, z));
    }

    void add_position(const vec3& diff) {
        set_position(position() + diff);
    }

    void add_position(const float dx, const float dy, const float dz = 0) {
        add_position(vec3(dx, dy, dz));
    }

    void set_rotation(const float degrees) {
        rotation_ = degrees;
        OnChangeTransform();
    }

    void set_scale_x(const float scale_x) {
        scale_x_ = scale_x;
        OnChangeTransform();
    }

    void set_scale_y(const float scale_y) {
        scale_y_ = scale_y;
        OnChangeTransform();
    }

    void set_scale(const float scale) {
        scale_x_ = scale;
        scale_y_ = scale;
        OnChangeTransform();
    }

    void set_skew_x(const float skew_x) {
        skew_x_ = skew_x;
        OnChangeTransform();
    }

    void set_skew_y(const float skew_y) {
        skew_y_ = skew_y;
        OnChangeTransform();
    }

    void set_z_order(int z_order) {
        z_order = z_order;
        is_reorder_child_dirty_ = true;
    }

    void set_anchor_point(const vec2& value) {
        if (anchor_point_ != value) {
            anchor_point_ = value;
            anchor_point_in_points_ = vec2(content_size_.x * anchor_point_.x,
                                           content_size_.y * anchor_point_.y);
            OnChangeTransform();
        }
    }
    
    void set_anchor_point(const float x, const float y) {
        set_anchor_point(vec2(x, 7));
    }

    void set_ignore_anchor_point_for_position(bool value) {
        if (ignore_anchor_point_for_position_ != value) {
            ignore_anchor_point_for_position_ = value;
            OnChangeTransform();
        }
    }

    void set_visible(const bool value) {
        visible_ = value;
        OnChangeVisible();
    }

    void Show() {
        set_visible(true);
    }

    void Hide() {
        set_visible(false);
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

    void set_actor_id(const ActorId& actor_id) {
        actor_id_ = actor_id;
    }

    void set_parent(const shared_ptr<Node>& value) {
        parent_ = value;
    }

    bool anchor_point_is_zero() const {
        return (anchor_point_.x == 0 && anchor_point_.y == 0);
    }

    vector<shared_ptr<Node> > children() const {
        return children_;
    }

    void Visit();

    void Enter() {
        for (vector<shared_ptr<Node> >::iterator i = children_.begin();
             i != children_.end(); ++i) {
            (*i)->Enter();
        }
        Resume();
        OnEnter();
    }

    void Exit()  {
        for (vector<shared_ptr<Node> >::iterator i = children_.begin();
             i != children_.end(); ++i) {
            (*i)->Exit();
        }
        Pause();
        OnExit();
    }

    void SortAllChildren();
    mat4 WorldTransform();
    vec3 WorldPosition();
    // void SetWorldPosition(const vec3& world_position);

    vec3 WorldPositionAt(const vec3& position);
    vec3 LocalPositionAt(const vec3& world_position);
    
    const Rect bounding_box() {
        Rect rect(0, 0, content_size_.x, content_size_.y);
        return rect * LocalTransform();
    }

    virtual void AddChild(const shared_ptr<Node>& child);
    virtual void RemoveChild(const shared_ptr<Node>& child);
    
    virtual bool Init() { return true; }
    virtual void Render()  {}

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void OnChangeTransform() {
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }

    virtual void OnChangeVisible() {
    }

    const mat4& LocalTransform();

protected:
    vec3 position_;
    float scale_x_;
    float scale_y_;
    float rotation_;
    float skew_x_;
    float skew_y_;
    vec2 anchor_point_;
    vec2 anchor_point_in_points_;
    bool ignore_anchor_point_for_position_;
    vec2 content_size_;
    int z_order_;
    bool visible_;

    mat4 local_transform_;
    mat4 local_inverse_;
    bool is_transform_dirty_;
    bool is_inverse_dirty_;
    bool is_reorder_child_dirty_;

    ActorId actor_id_;

    vector<shared_ptr<Node> > children_;
    weak_ptr<Node> parent_;
};

}

#endif /* defined(__sometuke__node_node__) */
