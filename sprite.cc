#include "sprite.h"
#include "shader_cache.h"

namespace kawaii {

Sprite::Sprite() {
    shader_program_ = ShaderCache::Shared()->get(kShader_PositionTextureColor);

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bottom_left.color  = tmp_color;
    quad_.bottom_right.color = tmp_color;
    quad_.top_left.color     = tmp_color;
    quad_.top_right.color    = tmp_color;
}

Sprite::Render() {
}

}
