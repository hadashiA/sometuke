#include "sprite.h"
#include "shader_cache.h"
#include "texture_cache.h"
#include "texture_2d.h"

namespace kawaii {

Sprite::Sprite()
    : color_(255, 255, 255),
      color_unmodified_(255, 255, 255) {
    shader_program_ = ShaderCache::Shared()->get(kShader_PositionTextureColor);

    std::memset(&quad_, 0, sizeof(quad_));

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bottom_left.color  = tmp_color;
    quad_.bottom_right.color = tmp_color;
    quad_.top_left.color     = tmp_color;
    quad_.top_right.color    = tmp_color;
}

void Sprite::set_color(const Color3B& value) {
    color_unmodified_ = value;
    if (does_opacity_modify_rgb_) {
        float opacity_factor = opacity_ / 255.0f;
        color_.r = value.r * opacity_factor;
        color_.g = value.g * opacity_factor;
        color_.b = value.b * opacity_factor;
    } else {
        color_ = value;
    }
    UpdateQuadColor();
}

void Sprite::set_opacity(GLubyte value) {
    opacity_ = value;
    if (does_opacity_modify_rgb_) {
        set_color(color_unmodified_);
    }
    UpdateQuadColor();
}

void Sprite::set_opacity_modify_rgb(bool value) {
    does_opacity_modify_rgb_ = value;
    Color3B old_color = color_;
    set_color(old_color);
}

void Sprite::set_texture(shared_ptr<Texture2D> value) {
    texture_ = value;
    if(!texture_ || !texture_->has_premultipled_alpha()) {
        set_blend_func_src(GL_SRC_ALPHA);
        set_blend_func_dst(GL_ONE_MINUS_SRC_ALPHA);
        set_opacity_modify_rgb(false);
    } else {
        set_blend_func_src(GL_ONE);
        set_blend_func_dst(GL_ONE_MINUS_SRC_ALPHA);
        set_opacity_modify_rgb(true);
    }
}

void Sprite::set_texture_rect(const Rect& rect) {
    set_texture_rect(rect, false, rect.size);
}

void Sprite::set_texture_rect(const Rect& rect, bool rotated,
                              const vec2& untrimmed_size) {
    set_content_size(untrimmed_size);
    vertex_rect_ = rect;
    UpdateQuadTexCoords();
}


void Sprite::Render() {
}

// private

void Sprite::UpdateQuadColor() {
    Color4B color4(color_, opacity_);
    quad_.bottom_left.color  = color4;
    quad_.bottom_right.color = color4;
    quad_.top_left.color     = color4;
    quad_.top_right.color    = color4;
}

void Sprite::UpdateQuadTexCoords() {
    
}

}
