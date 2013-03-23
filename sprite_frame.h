#ifndef __kawaii__sprite_frame__
#define __kawaii__sprite_frame__

#include "vertices.h"
#include "application.h"
#include "hashed_string.h"

#include "picojson/picojson.h"

#include <memory>
#include <string>

namespace kawaii {
using namespace std;

class Texture2D;

struct SpriteFrame {
public:
    SpriteFrame(shared_ptr<Texture2D> _texture, const Rect& _rect)
        : texture(_texture),
          rect(_rect),
          rotated(false),
          offset(0, 0),
          original_size(_rect.size),
          name("") {
        UpdatePixelSize();
    }

    SpriteFrame(shared_ptr<Texture2D> _texture,
                const Rect& _rect,
                bool _rotated,
                const vec2& _offset,
                const vec2& _original_size,
                const string& _name)
        : texture(_texture),
          rect(_rect),
          rotated(_rotated),
          offset(_offset),
          original_size(_original_size),
          name(_name) {
        UpdatePixelSize();
    }

    Rect rect;
    Rect rect_in_pixels;
    bool rotated;
    vec2 offset;
    vec2 offset_in_pixels;
    vec2 original_size;
    vec2 original_size_in_pixels;
    shared_ptr<Texture2D> texture;
    HashedString name;

private:
    void UpdatePixelSize() {
        float content_scale_factor = Application::Instance().content_scale_factor();
        rect_in_pixels = rect * content_scale_factor;
        offset_in_pixels = offset * content_scale_factor;
        original_size_in_pixels = original_size * content_scale_factor;
    }
};

}

#endif /* defined(__kawaii__sprite_frame__) */

