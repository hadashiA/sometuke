#include "sometuke/node/sprite.h"

#include "sometuke/shader_cache.h"
#include "sometuke/texture_cache.h"
#include "sometuke/texture_2d.h"
#include "sometuke/sprite_frame.h"
#include "sometuke/director.h"
#include "sometuke/process/animate.h"
#include "sometuke/drawing_primitives.h"
#include "sometuke/OpenGL_Internal.h"

#include "sometuke/process/sequence.h"
#include "sometuke/process/animate.h"

#include <algorithm>
#include <cassert>

namespace sometuke {

Sprite::Sprite()
    : color_(255, 255, 255),
      color_unmodified_(255, 255, 255),
      opacity_(255),
      does_opacity_modify_rgb_(false),
      vertex_rect_rotated_(false),
      fliped_x_(false),
      fliped_y_(false),
      offset_position_(0, 0),
      unflipped_offset_position_from_center_(0, 0) {
    shader_program_ = ShaderCache::Instance()[kShader_PositionTextureColor];

    std::memset(&quad_, 0, sizeof(quad_));

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bl.color = tmp_color;
    quad_.br.color = tmp_color;
    quad_.tl.color = tmp_color;
    quad_.tr.color = tmp_color;
}

bool Sprite::InitWithFilename(const string& path) {
    shared_ptr<Texture2D> texture = TextureCache::Instance().FetchFromPath(path);
    return InitWithTexture(texture);
}

bool Sprite::InitWithTexture(const shared_ptr<Texture2D>& texture) {
    return InitWithTexture(texture, Rect(vec2(0, 0), texture->content_size()), false);
}

bool Sprite::InitWithTexture(const shared_ptr<Texture2D>& texture,
                  const Rect& rect, bool rotated) {
    set_texture(texture);
    set_texture_rect(rect, rotated, rect.size);
    return true;
}

bool Sprite::InitWithSpriteFrame(const shared_ptr<SpriteFrame>& frame) {
    return InitWithSpriteFrame(*frame.get());
}

bool Sprite::InitWithSpriteFrame(const SpriteFrame& frame) {
    bool result = InitWithTexture(frame.texture,
                                  frame.rect,
                                  frame.rotated);
    if (!result) {
        return false;
    }
    set_display_frame(frame);
    
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

void Sprite::set_texture(const shared_ptr<Texture2D>& value) {
    texture_ = value;

    if (texture_->has_premultiplied_alpha()) {
        blend_func_src_ = GL_ONE;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        set_opacity_modify_rgb(true);
    } else {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        set_opacity_modify_rgb(false);
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

    // TODO: offset_position calculate
    vec2 relative_offset(unflipped_offset_position_from_center_);
    if (fliped_x_) {
        relative_offset.x = -relative_offset.x;
    }
    if (fliped_y_) {
        relative_offset.y = -relative_offset.y;
    }
    offset_position_.x = relative_offset.x + (content_size().x - rect.size.x) / 2;
    offset_position_.y = relative_offset.y + (content_size().y - rect.size.y) / 2;

    float x1 = offset_position_.x;
    float y1 = offset_position_.y;
    float x2 = x1 + rect.size.x;
    float y2 = y1 + rect.size.y;

    // Don't update Z
    quad_.bl.pos = vec3(x1, y1, 0);
    quad_.br.pos = vec3(x2, y1, 0);
    quad_.tl.pos = vec3(x1, y2, 0);
    quad_.tr.pos = vec3(x2, y2, 0);
}

void Sprite::set_display_frame(weak_ptr<SpriteFrame> weak_frame) {
    if (shared_ptr<SpriteFrame> frame = weak_frame.lock()) {
        set_display_frame(*frame.get());
    }
}

void Sprite::set_display_frame(const SpriteFrame& frame) {
    unflipped_offset_position_from_center_ = frame.offset;
    texture_ = frame.texture;
    vertex_rect_rotated_ = frame.rotated;
        
    set_texture_rect(frame.rect, vertex_rect_rotated_, frame.original_size);
}

void Sprite::Render() {
    shader_program_->Use();
    shader_program_->SetUniformsForBuiltins();
    
    // set blending
    if (blend_func_src_ == GL_ONE && blend_func_dst_ == GL_ZERO) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(blend_func_src_, blend_func_dst_);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id());

    // attributes
    glEnableVertexAttribArray(kVertexAttrib_Position);
    glEnableVertexAttribArray(kVertexAttrib_Color);
    glEnableVertexAttribArray(kVertexAttrib_TexCoords);

#define VERTEX_SIZE sizeof(quad_.bl)
    long offset = (long)&quad_;
    unsigned int diff;

    // position
    diff = offsetof(P3F_C4B_T2F, pos);
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (void *)(offset + diff));

    // texCoord
    diff = offsetof(P3F_C4B_T2F, tex_coord);
    glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (void *)(offset + diff));

    // color
    diff = offsetof(P3F_C4B_T2F, color);
    glVertexAttribPointer(kVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE,
                          (void *)(offset + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_GL_ERROR_DEBUG();

#ifdef KAWAII_DEBUG
    if (Director::Instance().director().debug_drawing()) {
        vec2 vertices[4] = {
            vec2(quad_.bl.pos.x, quad_.bl.pos.y),
            vec2(quad_.br.pos.x, quad_.br.pos.y),
            vec2(quad_.tr.pos.x, quad_.tr.pos.y),
            vec2(quad_.tl.pos.x, quad_.tl.pos.y),
        };
        DrawPoly(vertices, 4, true);
    }
#endif

    glDisableVertexAttribArray(kVertexAttrib_Position);
    glDisableVertexAttribArray(kVertexAttrib_Color);
    glDisableVertexAttribArray(kVertexAttrib_TexCoords);
}

// private

void Sprite::UpdateQuadColor() {
    Color4B color4(color_, opacity_);
    quad_.bl.color = color4;
    quad_.br.color = color4;
    quad_.tl.color = color4;
    quad_.tr.color = color4;
}

void Sprite::UpdateQuadTexCoords() {
    Rect rect = vertex_rect_ * Director::Instance().content_scale_factor();
    float atlas_width  = texture_->pixel_size().x;
    float atlas_height = texture_->pixel_size().y;
    float left, right, top, bottom;

    if (vertex_rect_rotated_) {
        // FIX_ARTIFACTS_BY_STRECHIN_TEXEL ??
        left   = rect.pos.x / atlas_width;
        right  = (rect.pos.x + rect.size.y) / atlas_width;
        top    = rect.pos.y / atlas_height;
        bottom = (rect.pos.y + rect.size.x) / atlas_height;
        
        if (fliped_x_) {
            std::swap(top, bottom);
        }
        if (fliped_y_) {
            std::swap(left, right);
        }
        
        quad_.bl.tex_coord.u = left;
        quad_.bl.tex_coord.v = top;
        quad_.br.tex_coord.u = left;
        quad_.br.tex_coord.v = bottom;
        quad_.tl.tex_coord.u = right;
        quad_.tl.tex_coord.v = top;
        quad_.tr.tex_coord.u = right;
        quad_.tr.tex_coord.v = bottom;

    } else {
        // FIX_ARTIFACTS_BY_STRECHING_TEXEL ??
        left   = rect.pos.x / atlas_width;
        right  = (rect.pos.x + rect.size.x) / atlas_width;
        top    = rect.pos.y / atlas_height;
        bottom = (rect.pos.y + rect.size.y) / atlas_height;

        if (fliped_x_) {
            std::swap(left, right);
        }
        if (fliped_y_) {
            std::swap(top, bottom);
        }

        quad_.bl.tex_coord.u = left;
        quad_.bl.tex_coord.v = bottom;
        quad_.br.tex_coord.u = right;
        quad_.br.tex_coord.v = bottom;
        quad_.tl.tex_coord.u = left;
        quad_.tl.tex_coord.v = top;
        quad_.tr.tex_coord.u = right;
        quad_.tr.tex_coord.v = top;
    }
}

}

