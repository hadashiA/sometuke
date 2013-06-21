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

        if (ignore_anchor_point_for_position_) {
            x += anchor_point_in_points_.x;
            y += anchor_point_in_points_.y;
        }

        float c = 1;
        float s = 0;
        if (rotation_) {
            float radians = -DegreesToRadians(rotation_);
            c = cosf(radians);
            s = sinf(radians);
        }

        // optimization:
        // inline anchor point calculation if skew is not needed
        bool needs_skew_matrix = (skew_x_ || skew_y_);
        if (!needs_skew_matrix && !anchor_point_is_zero()) {
            x += c * -anchor_point_in_points_.x * scale_x_ + -s * -anchor_point_in_points_.y * scale_y_;
            y += s * -anchor_point_in_points_.x * scale_x_ +  c * -anchor_point_in_points_.y * scale_y_;
        }

        // Build Transform Matrix
        float a_a  = c * scale_x_;
        float a_b  = s * scale_x_;
        float a_c  = -s * scale_y_;
        float a_d  = c * scale_y_;
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

void Node::RemoveChild(shared_ptr<Node> child) {
    if (child) {
        for (vector<shared_ptr<Node> >i = children_.begin(); i != children_.end();) {
            if (*it == child) {
                it = children_.erase(it);
                break;
            } else {
                ++it;
            }
        }
    }
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();

    model_view = LocalTransform() * model_view;

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
    sequence_ = NULL;
}

void Node::Loop(int num) {
    shared_ptr<class Repeat> repeat(new class Repeat(sequence(), num));
    process_manager().Attach(shared_from_this(), repeat);
    sequence_ = NULL;
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
