#ifndef __sometuke__ios_touch_dispatcher__
#define __sometuke__ios_touch_dispatcher__

#include "sometuke/director.h"
#include "sometuke/memory_pool.h"

#include <unordered_set>
#include <vector>
#include <map>
#include <memory>

namespace sometuke {
using namespace std;

class Node;

typedef unsigned long TouchId;

typedef enum {
    kTouchBegan,
    kTouchMoved,
    kTouchEnded,
    kTouchCancelled,
} TouchPhase;

struct Touch {
    static const EventType TYPE;

    Touch(TouchId touch_id, TouchPhase _phase, const vec2& loc, const vec2& prev, size_t cnt)
        : id(touch_id),
          phase(_phase),
          point(loc),
          prev_point(prev),
          tap_count(cnt),
          timestamp(std::time(NULL)) {
    }

    TouchId id;
    TouchPhase phase;
    vec2 point;
    vec2 prev_point;
    size_t tap_count;
    time_t timestamp;
};

typedef vector<shared_ptr<Touch> > TouchSet;
    
class TargetedTouchListener : public enable_shared_from_this<TargetedTouchListener> {
public:
    TargetedTouchListener()
        : paused_(false) {
    }

    virtual ~TargetedTouchListener() {}

    bool TouchStart(const shared_ptr<Touch>& touch) {
        bool claimed = TouchBegan(touch);
        if (claimed) {
            claimed_touch_ids_.insert(touch->id);
        }
        return claimed;
    }

    void TouchNext(const shared_ptr<Touch>& touch, TouchPhase phase) {
        unordered_set<TouchId>::iterator iter = claimed_touch_ids_.find(touch->id);
        if (iter != claimed_touch_ids_.end()) {
            switch (phase) {
            case kTouchMoved:
                TouchMoved(touch);
                break;
            case kTouchEnded:
                TouchEnded(touch);
                claimed_touch_ids_.erase(iter);
                break;
            case kTouchCancelled:
                TouchCancelled(touch);
                claimed_touch_ids_.erase(iter);
                break;
            default:
                break;
            }
        }
    }

    void Pause()  { paused_ = true; }
    void Resume() { paused_ = false; }
    const bool paused() const { return paused_; }

    virtual void Listen(int priority);

    virtual const bool shallows_touches() const = 0;

    virtual bool TouchBegan(const shared_ptr<Touch>& touch) = 0;
    virtual void TouchMoved(const shared_ptr<Touch>& touch) = 0;
    virtual void TouchEnded(const shared_ptr<Touch>& touch) = 0;
    virtual void TouchCancelled(const shared_ptr<Touch>& touch) = 0;

private:
    unordered_set<TouchId> claimed_touch_ids_;
    bool paused_;
};

class StandardTouchListener : public enable_shared_from_this<StandardTouchListener> {
public:
    StandardTouchListener()
        : paused_(false) {
    }
    
    virtual ~StandardTouchListener() {}

    void Pause()  { paused_ = true; }
    void Resume() { paused_ = false; }
    
    const bool paused() const { return paused_; };

    virtual void Listen(int priority);

    virtual void TouchesBegan(const TouchSet& touches) = 0;
    virtual void TouchesMoved(const TouchSet& touches) = 0;
    virtual void TouchesEnded(const TouchSet& touches) = 0;
    virtual void TouchesCancelled(const TouchSet& touches) = 0;

private:
    bool paused_;
};
    
typedef multimap<int, weak_ptr<StandardTouchListener> > StandardTouchListenerTable;
typedef multimap<int, weak_ptr<TargetedTouchListener> > TargetedTouchListenerTable;

class TouchDispatcher {
public:    
    static TouchDispatcher& Instance() {
        static unique_ptr<TouchDispatcher> __instance;
        if (!__instance) {
            __instance.reset(new TouchDispatcher);
        }
        return *__instance;
    }

    TouchDispatcher() : enabled_(true) {}

    bool enabled() { return enabled_; }
    void Enable()  { enabled_ = true; }
    void Disable() { enabled_ = false; }

    void AddListener(const weak_ptr<StandardTouchListener>& listener, int priority) {
        pair<int, weak_ptr<StandardTouchListener> > val(priority, listener);
        standard_listeners_.insert(val);
    }
        
    void AddListener(const weak_ptr<TargetedTouchListener>& listener, int priority) {
        pair<int, weak_ptr<TargetedTouchListener> > val(priority, listener);
        targeted_listeners_.insert(val);
    }

    void RemoveAllListeners() {
        standard_listeners_.clear();
        targeted_listeners_.clear();
    }
    void RemoveListener(const shared_ptr<StandardTouchListener>& listener);
    void RemoveListener(const shared_ptr<TargetedTouchListener>& listener);

    void Trigger(TouchPhase phase, TouchSet touches);

    // vec2 ConvertToGL(const vec2 touch_point) {
    //     vec2 win_size = Director::Instance().size_in_points();
    //     return vec2(touch_point.x, win_size.y - touch_point.y);
    // }
    vec2 ConvertToGL(const vec2 touch_point);

private:
    bool enabled_;
    TargetedTouchListenerTable targeted_listeners_;
    StandardTouchListenerTable standard_listeners_;
};

}

#endif /* defined(__sometuke__ios_touch_dispatcher__) */
