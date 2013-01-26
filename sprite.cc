#include "sprite.h"

#include "shader_cache.h"
#include "texture_cache.h"
#include "texture_2d.h"
#include "application.h"

#include <algorithm>
#include <cassert>

namespace kawaii {

Sprite::Sprite()
    : color_(255, 255, 255),
      color_unmodified_(255, 255, 255),
      vertex_rect_rotated_(false) {
    shader_program_ = ShaderCache::Shared()->get(kShader_PositionTextureColor);

    std::memset(&quad_, 0, sizeof(quad_));

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bottom_left.color  = tmp_color;
    quad_.bottom_right.color = tmp_color;
    quad_.top_left.color     = tmp_color;
    quad_.top_right.color    = tmp_color;
}

bool Sprite::InitWithTexture(shared_ptr<Texture2D> texture) {
    return InitWithTexture(texture, Rect(vec2(0, 0), texture->content_size()), false);
}

bool Sprite::InitWithTexture(shared_ptr<Texture2D> texture,
                             const Rect& rect, bool rotated) {
    set_texture(texture);
    set_texture_rect(rect, rotated, rect.size);
    return true;
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
    vertex_rect_rotated_ = rotated;
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
    Rect rect = vertex_rect_ * Application::Current()->content_scale_factor();
    float atlas_width  = texture_->pixel_size().x;
    float atlas_height = texture_->pixel_size().y;
    float left, right, top, bottom;

    if (vertex_rect_rotated_) {
        assert(false);          // no implemented
    } else {
        // FIX_ARTIFACTS_BY_STRECHING_TEXEL ??
        left   = rect.pos.x / atlas_width;
        right  = (rect.pos.x + rect.size.x) / atlas_width;
        top    = rect.pos.y / atlas_height;
        bottom = (rect.pos.y + rect.size.y) / atlas_height;

        if (fliped_.x) {
            std::swap(left, right);
        }
        if (fliped_.y) {
            std::swap(top, bottom);
        }

        quad_.bottom_left.tex_coord.u  = left;
        quad_.bottom_left.tex_coord.v  = bottom;
        quad_.bottom_right.tex_coord.u = right;
        quad_.bottom_right.tex_coord.v = bottom;
        quad_.top_left.tex_coord.u     = left;
        quad_.top_left.tex_coord.v     = top;
        quad_.top_right.tex_coord.u    = right;
        quad_.top_right.tex_coord.v    = top;
    }
}

}
