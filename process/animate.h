#ifndef __kawaii__process_animate__
#define __kawaii__process_animate__

#include "process.h"
#include "animation.h"

#include <memory>

namespace kawaii {
    
class Sprite;

class Animate : public Process {
public:
    static const HashedString TYPE;
    Animate(shared_ptr<Sprite> target, shared_ptr<Animation> animation) :
        animation_(animation),
        frame_num_(0),
        elapsed_(0),
        target_(target),
        original_frame_(),
        executed_loops_(0) {
    }

    virtual const HashedString& type() {
        return Animate::TYPE;
    }
    
    virtual void OnEnter();
    virtual void Update(const ii_time delta_time);
    virtual void Kill();

private:
    shared_ptr<Sprite> target_;
    shared_ptr<SpriteFrame> original_frame_;
    unsigned int frame_num_;
    shared_ptr<Animation> animation_;
    ii_time elapsed_;
    unsigned int executed_loops_;
};

}

#endif /* defined(__kawaii__process_animate__) */
