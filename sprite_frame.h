#ifndef __skidarak__sprite_frame__
#define __skidarak__sprite_frame__

#include "skidarak/vertices.h"
#include "skidarak/application.h"
#include "skidarak/hashed_string.h"

#include "skidarak/picojson/picojson.h"

#include <memory>
#include <string>

namespace skidarak {
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
                const Rect& _rect_in_pixels,
                bool _rotated,
                const vec2& _offset_in_pixels,
                const vec2& _original_size_in_pixels,
                const string& _name)
        : texture(_texture),
          rect_in_pixels(_rect_in_pixels),
          rotated(_rotated),
          offset_in_pixels(_offset_in_pixels),
          original_size_in_pixels(_original_size_in_pixels),
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
        rect          = rect_in_pixels / content_scale_factor;
        offset        = offset_in_pixels / content_scale_factor;
        original_size = original_size_in_pixels / content_scale_factor;
    }
};

}

#endif /* defined(__skidarak__sprite_frame__) */

