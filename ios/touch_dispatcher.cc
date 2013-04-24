#include "touch_dispatcher.h"

#include "kawaii/logger.h"
#include "kawaii/node/node.h"
#include "kawaii/matrix_stack.h"
#include "kawaii/application.h"

namespace kawaii {

void TouchDispatcher::AddStandardListener(shared_ptr<StandardTouchListener> listener, int priority) {
    pair<int, shared_ptr<StandardTouchListener> > val(priority, listener);
    standard_listeners_.insert(val);
}

void TouchDispatcher::AddTargetedListener(shared_ptr<TargetedTouchListener> listener, int priority) {
    pair<int, shared_ptr<TargetedTouchListener> > val(priority, listener);
    targeted_listeners_.insert(val);
}

void TouchDispatcher::RemoveListener(shared_ptr<StandardTouchListener> listener) {
    for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
         i != standard_listeners_.end();) {
        if (listener == i->second) {
            standard_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::RemoveListener(shared_ptr<TargetedTouchListener> listener) {
    for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
         i != targeted_listeners_.end();) {
        if (listener == i->second) {
            targeted_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::Trigger(TouchPhase phase, TouchEventSet touches) {
    for (TouchEventSet::iterator touch_iter = touches.begin(); touch_iter != touches.end(); ++touch_iter) {
        shared_ptr<TouchEvent> touch = *touch_iter;

        unordered_set<TouchId> shallows_touch_ids;

        // process the target handlers 1st
        if (!targeted_listeners_.empty()) {
            for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
                 i != targeted_listeners_.end();) {
                shared_ptr<TargetedTouchListener> listener = i->second;
                if (listener->listening()) {
                    bool claimed = false;
                    if (phase == kTouchBegan) {
                        claimed = listener->TouchStart(touch);
                    } else {
                        listener->TouchNext(touch, phase);
                    }
                    if (claimed && listener->shallows_touches()) {
                        shallows_touch_ids.insert(touch->id);
                    }
                
                    ++i;
                } else {
                    targeted_listeners_.erase(i++);
                }
            }
        }

        // process standard handlers 2nd
        if (!standard_listeners_.empty() &&
            shallows_touch_ids.find(touch->id) == shallows_touch_ids.end()) {
            for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
                 i != standard_listeners_.end();) {
                shared_ptr<StandardTouchListener> listener = i->second;
                if (listener->listening()) {
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
                    ++i;
                } else {
                    standard_listeners_.erase(i++);
                }
            }
        }
    }
}

vec2 TouchDispatcher::ConvertToGL(const vec2 touch_location) {
    mat4& projection = MatrixStack<GLProjection>::Instance().Top();
    mat4& model_view = MatrixStack<GLModelView>::Instance().Top();
    mat4 transform = model_view * projection;
    mat4 inversed  = transform.Inverse();

    float z_clip = transform.w.z / transform.w.w;
    vec2 gl_size = Application::Instance().size_in_points();

    vec3 clip_coord(2.0 * touch_location.x / gl_size.x - 1.0,
                    1.0 - 2.0 * touch_location.y / gl_size.y,
                    z_clip);

    vec4 gl_coord = vec4(clip_coord, 1) * inversed;
    vec3 result(gl_coord.x / gl_coord.w,
                gl_coord.y / gl_coord.w,
                gl_coord.z / gl_coord.w);

    return vec2(result.x, result.y);
}

}
