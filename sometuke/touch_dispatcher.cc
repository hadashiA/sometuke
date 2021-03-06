#include "sometuke/touch_dispatcher.h"

#include "sometuke/logger.h"
#include "sometuke/node/node.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/director.h"

namespace sometuke {

void TargetedTouchListener::Listen(int priority) {
    TouchDispatcher::Instance().AddListener(shared_from_this(), priority);
}

void StandardTouchListener::Listen(int priority) {
    TouchDispatcher::Instance().AddListener(shared_from_this(), priority);
}

void TouchDispatcher::RemoveListener(const shared_ptr<StandardTouchListener>& listener) {
    for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
         i != standard_listeners_.end();) {
        if (listener == i->second.lock()) {
            standard_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::RemoveListener(const shared_ptr<TargetedTouchListener>& listener) {
    for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
         i != targeted_listeners_.end();) {
        if (listener == i->second.lock()) {
            targeted_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::Trigger(TouchPhase phase, TouchSet touches) {
    for (TouchSet::iterator touch_iter = touches.begin(); touch_iter != touches.end(); ++touch_iter) {
        shared_ptr<Touch> touch = *touch_iter;

        unordered_set<TouchId> shallows_touch_ids;

        // process the target handlers 1st
        if (!targeted_listeners_.empty()) {
            for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
                 i != targeted_listeners_.end();) {
                if (i->second.expired()) {
                    targeted_listeners_.erase(i++);
                } else {
                    shared_ptr<TargetedTouchListener> listener = i->second.lock();
                    if (!listener->paused()) {
                        bool claimed = false;
                        if (phase == kTouchBegan) {
                            claimed = listener->TouchStart(touch);
                        } else {
                            listener->TouchNext(touch, phase);
                        }
                        if (claimed && listener->shallows_touches()) {
                            shallows_touch_ids.insert(touch->id);
                        }
                    }
                    ++i;
                }
            }
        }

        // process standard handlers 2nd
        if (!standard_listeners_.empty() &&
            shallows_touch_ids.find(touch->id) == shallows_touch_ids.end()) {
            for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
                 i != standard_listeners_.end();) {
                if (i->second.expired()) {
                    standard_listeners_.erase(i++);
                } else {
                    shared_ptr<StandardTouchListener> listener = i->second.lock();
                    if (!listener->paused()) {
                        switch (phase) {
                        case kTouchBegan:
                            listener->TouchesBegan(touches);
                            break;
                        case kTouchMoved:
                            listener->TouchesMoved(touches);
                            break;
                        case kTouchEnded:
                            listener->TouchesEnded(touches);
                            break;
                        case kTouchCancelled:
                            listener->TouchesCancelled(touches);
                            break;
                        }
                    }
                    ++i;
                }
            }
        }
    }
}

vec2 TouchDispatcher::ConvertToGL(const vec2 touch_point) {
    mat4& projection = MatrixStack<GLProjection>::Instance().Top();
    mat4& model_view = MatrixStack<GLModelView>::Instance().Top();
    mat4 transform = model_view * projection;
    mat4 inversed  = transform.Inverse();

    float z_clip = transform.w.z / transform.w.w;
    vec2 gl_size = Director::Instance().size_in_points();

    vec3 clip_coord(2.0 * touch_point.x / gl_size.x - 1.0,
                    1.0 - 2.0 * touch_point.y / gl_size.y,
                    z_clip);

    vec4 gl_coord = vec4(clip_coord, 1) * inversed;
    vec3 result(gl_coord.x / gl_coord.w,
                gl_coord.y / gl_coord.w,
                gl_coord.z / gl_coord.w);

    return vec2(result.x, result.y);
}

}
