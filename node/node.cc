#include "kawaii/node/node.h"

#include "kawaii/matrix_stack.h"
#include "kawaii/logger.h"

#include "kawaii/process/sequence.h"
#include "kawaii/process/repeat.h"
#include "kawaii/process/move_to.h"
#include "kawaii/process/move_by.h"
#include "kawaii/process/delay.h"

#include <cassert>

namespace kawaii {
using namespace std;

const mat4& Node::LocalTransform() {
    if (is_transform_dirty_) {
        float x = position_.x;
        float y = position_.y;
        float z = position_.z;

        float cx = 1;
        float sx = 0;
        float cy = 1;
        float sy = 0;
        if (rotation_x_ || rotation_y_) {
            float radians_x_ = -DegreesToRadians(rotation_x_);
            float radians_y_ = -DegreesToRadians(rotation_y_);
            cx = cosf(radians_x_);
            sx = sinf(radians_x_);
            cy = cosf(radians_y_);
            sy = sinf(radians_y_);
        }

        // optimization:
        // inline anchor point calculation if skew is not needed
        bool needs_skew_matrix = (skew_x_ || skew_y_);
        if (!needs_skew_matrix && !anchor_point_is_zero()) {
            x += cy * -anchor_point_in_points_.x * scale_x_ + -sx * -anchor_point_in_points_.y * scale_y_;
            y += sy * -anchor_point_in_points_.x * scale_x_ +  cx * -anchor_point_in_points_.y * scale_y_;
        }

        // Build Transform Matrix
        float a_a  = cy * scale_x_;
        float a_b  = sy * scale_x_;
        float a_c  = -sx * scale_y_;
        float a_d  = cx * scale_y_;
        local_transform_ = mat4(a_a, a_b, 0, 0,
                                a_c, a_d, 0, 0,
                                  0,   0, 1, 0,
                                  x,   y, z, 1);

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needs_skew_matrix) { 
            mat4 skew_matrix(1.0f, tanf(DegreesToRadians(skew_y_)), 0, 0,
                             tanf(DegreesToRadians(skew_x_)), 1.0f, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
            local_transform_ = skew_matrix * local_transform_;

            // adjust anchor point
            if (!anchor_point_is_zero()) {
                local_transform_ = local_transform_.Translate(-anchor_point_in_points_.x, -anchor_point_in_points_.y, 0);
            }
        }

        is_transform_dirty_ = false;
    }

    return local_transform_;
}

mat4 Node::WorldTransform() {
    mat4 t = LocalTransform();
    for (shared_ptr<Node> p = parent(); p; p = p->parent()) {
        t *= p->LocalTransform();
    }
    return t;
}

vec3 Node::WorldPosition() {
    return WorldPositionAt(position_);
}

vec3 Node::WorldPositionAt(const vec3& position) {
    vec4 pos = vec4(position, 1) * WorldTransform();
    return vec3(pos.x, pos.y, pos.z);
}

vec3 Node::LocalPositionAt(const vec3& world_position) {
    return WorldPosition() - world_position;
}

void Node::AddChild(shared_ptr<Node> child) {
    assert(!child->parent());

    child->set_parent(shared_from_this());
    children_.push_back(child);

    if (!paused_) {
        child->OnEnter();
    }
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();

    model_view *= LocalTransform();

    if (!children_.empty()) {
        Render();

        // sort all children

        // Render children z_order < 0

        // Render();

        
        // Render children z_order > 0
        for (vector<shared_ptr<Node> >::iterator iter = children_.begin(); iter != children_.end(); iter++) {
            shared_ptr<Node> child = (*iter);
            child->Visit();
        }

    } else {
        Render();
    }

    MatrixStack<GLModelView>::Instance().Pop();
}


// process helpers
shared_ptr<Sequence> Node::sequence() {
    if (!sequence_) {
        sequence_.reset(new Sequence);
    }
    return sequence_;
}

void Node::Run() {
    process_manager().Attach(shared_from_this(), sequence());
    sequence().reset();
}

void Node::Loop(int num) {
    shared_ptr<class Repeat> repeat(new class Repeat(sequence(), num));
    process_manager().Attach(shared_from_this(), repeat);
    sequence().reset();
}

void Node::StopAllProcess() {
    process_manager().Detach(shared_from_this());
}

shared_ptr<Node> Node::MoveTo(const ii_time duration, const vec3& to) {
    shared_ptr<class MoveTo> move_to(new class MoveTo(duration, to));
    sequence()->Push(move_to);

    return shared_from_this();
}

shared_ptr<Node> Node::MoveBy(const ii_time duration, const vec3& delta) {
    shared_ptr<class MoveBy> move_by(new class MoveBy(duration, delta));
    sequence()->Push(move_by);

    return shared_from_this();
}

shared_ptr<Node> Node::Delay(const ii_time duration) {
    shared_ptr<class Delay> delay(new class Delay(duration));
    sequence()->Push(delay);

    return shared_from_this();
}

shared_ptr<Node> Node::Repeat(int num) {
    shared_ptr<Process> last = sequence()->Pop();
    shared_ptr<class Repeat> repeat(new class Repeat(last, num));
    sequence()->Push(repeat);

    return shared_from_this();
}

}
