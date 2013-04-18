#ifndef __kawaii__ios_touch_dispatcher__
#define __kawaii__ios_touch_dispatcher__

#include "kawaii/ios/events.h"

#include <unordered_set>
#include <vector>
#include <map>
#include <memory>

namespace kawaii {
using namespace std;

class Node;

typedef vector<shared_ptr<TouchEvent> > TouchEventSet;
    
class TargetedTouchListener {
public:
    TargetedTouchListener(shared_ptr<Node> handler, bool shallows_touches = false)
        : handler_(handler),
          shallows_touches_(shallows_touches) {
    }

    virtual ~TargetedTouchListener() {}

    shared_ptr<Node> handler() const { return handler_.lock(); }

    void TouchStart(shared_ptr<TouchEvent> touch) {
        bool claimed = TouchBegan(touch);
        if (claimed) {
            claimed_touch_ids_.insert(touch->id);
        }
    }

    void TouchNext(shared_ptr<TouchEvent> touch, TouchPhase phase) {
        claimed_touch_ids_.find(touch->id);
    }

    virtual bool TouchBegan(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchMoved(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchEnded(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchCancelled(shared_ptr<TouchEvent> touch) = 0;

private:
    weak_ptr<Node> handler_;
    unordered_set<TouchId> claimed_touch_ids_;
    bool shallows_touches_;
};

class StandardTouchListener {
public:
    StandardTouchListener(shared_ptr<Node> handler) : handler_(handler) {}
    virtual ~StandardTouchListener() {}

    shared_ptr<Node> handler() const { return handler_.lock(); }

    virtual void TouchesBegan(TouchEventSet touches) = 0;
    virtual void TouchesMoved(TouchEventSet touches) = 0;
    virtual void TouchesEnded(TouchEventSet touches) = 0;
    virtual void TouchesCancelled(TouchEventSet touches) = 0;

private:
    weak_ptr<Node> handler_;
};
    
typedef multimap<int, shared_ptr<StandardTouchListener> > StandardTouchListenerTable;
typedef multimap<int, shared_ptr<TargetedTouchListener> > TargetedTouchListenerTable;

class TouchDispatcher {
public:    
    static TouchDispatcher& Instance() {
        static unique_ptr<TouchDispatcher> __instance;
        if (!__instance) {
            __instance.reset(new TouchDispatcher);
        }
        return *__instance;
    }

    TouchDispatcher() : enabled_(true) {
    }

    bool enabled() { return enabled_; }
    void Enable()  { enabled_ = true; }
    void Disable() { enabled_ = false; }

    void AddStandardListener(shared_ptr<StandardTouchListener> listener, int priority);
    void AddTargetedListener(shared_ptr<TargetedTouchListener> listener, int priority);

    void RemoveAllListeners() {
        standard_listeners_.clear();
        targeted_listeners_.clear();
    }
    void RemoveListener(shared_ptr<StandardTouchListener> listener);
    void RemoveListener(shared_ptr<TargetedTouchListener> listener);

    void Trigger(TouchPhase phase, TouchEventSet touches);

private:
    bool enabled_;
    TargetedTouchListenerTable targeted_listeners_;
    StandardTouchListenerTable standard_listeners_;
};

}

#endif /* defined(__kawaii__ios_touch_dispatcher__) */
