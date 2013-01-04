#ifndef __kawaii__node__
#define __kawaii__node__

#include "vector.h"
#include "shader_cache.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class Node {
public:
    Node()
        : position_(0, 0, 0),
          scale_(1, 1),
          rotation_(0, 0),
          anchor_point_(0, 0),
          content_size_(0, 0),
          z_order_(0),
          visible_(true),
          is_reorder_child_dirty_(false) {
    }
    
    virtual ~Node() {}

    const vec3& position() const {
        return position_;
    }

    void set_position(const vec3& value) {
        position_ = value;
    }

    const vec2& scale() const {
        return scale_;
    }

    void set_scale(const vec2& value) {
        scale_ = value;
    }

    const int z_order() const {
        return z_order_;
    }

    void set_z_order(int value) {
        z_order_ = value;
    }

    virtual void AddChild(shared_ptr<Node> child);

    virtual Node& operator<<(shared_ptr<Node> child) {
        AddChild(child);
        return *this;
    }

    virtual void OnEnter() = 0;
    virtual void Render() = 0;

private:    
    vec3 position_;
    vec2 scale_;
    vec2 rotation_;
    vec2 anchor_point_;
    vec2 content_size_;
    int z_order_;
    bool visible_;

    bool is_reorder_child_dirty_;

    vector<shared_ptr<Node> > children_;
};

}

#endif /* defined(__kawaii__node__) */
