#include "sprite.h"
#include "shader_cache.h"
#include "texture_cache.h"
#include "texture_2d.h"

namespace kawaii {

Sprite::Sprite() {
    shader_program_ = ShaderCache::Shared()->get(kShader_PositionTextureColor);

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bottom_left.color  = tmp_color;
    quad_.bottom_right.color = tmp_color;
    quad_.top_left.color     = tmp_color;
    quad_.top_right.color    = tmp_color;
}

void Sprite::Render() {
}

// private

void Sprite::UpdateBlendFunc() {
    if (!texture_ || !texture_->has_premultipled_alpha()) {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        
    } else {
        
    }
}

}
