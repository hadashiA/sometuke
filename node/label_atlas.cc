#include "kawaii/node/label_atlas.h"

#include "kawaii/drawing_primitives.h"

namespace kawaii {

bool LabelAtlas::InitWithText(const string& text,
                              const string& texture_path,
                              size_t width, size_t height,
                              char map_start_char) {
    if (!AtlasNode::InitWithTileFile(texture_path, width, height, text.length())){
        return false;
    }

    map_start_char_ = map_start_char;
    set_text(text);
    
    return true;
}

void LabelAtlas::UpdateAtlasValues() {
    size_t n = text_.length();
    Quad_P3F_C4B_T2F quad;
    const char *s = text_.c_str();

    shared_ptr<Texture2D> texture = texture_atlas_->texture();
    vec2 pixel_size = texture->pixel_size();

    float content_scale_factor = Application::Instance().content_scale_factor();
    float item_width_in_pixels  = item_width_  * content_scale_factor;
    float item_height_in_pixels = item_height_ * content_scale_factor;

    for (int i = 0; i < n; ++i) {
        unsigned char a = s[i] - map_start_char_;
        float row = (a % items_per_row_);
        float col = (a / items_per_row_);
        
        float left   = row  * item_width_in_pixels / pixel_size.x;
        float right  = left + item_width_in_pixels / pixel_size.x;
        float top    = col  * item_height_in_pixels / pixel_size.y;
        float bottom = top  + item_height_in_pixels / pixel_size.y;

        quad.top_left.tex_coord.u     = left;
        quad.top_left.tex_coord.v     = top;
        quad.top_right.tex_coord.u    = right;
        quad.top_right.tex_coord.v    = top;
        quad.bottom_left.tex_coord.u  = left;
        quad.bottom_left.tex_coord.v  = bottom;
        quad.bottom_right.tex_coord.u = right;
        quad.bottom_right.tex_coord.v = bottom;

        quad.bottom_left.pos.x  = (int)(i * item_width_);
        quad.bottom_left.pos.y  = 0;
        quad.bottom_left.pos.z  = 0.0f;
        quad.bottom_right.pos.x = (int)(i * item_width_ + item_width_);
        quad.bottom_right.pos.y = 0;
        quad.bottom_right.pos.z = 0.0f;
        quad.top_left.pos.x     = (int)(i * item_width_);
        quad.top_left.pos.y     = (int)(item_height_);
        quad.top_left.pos.z     = 0.0f;
        quad.top_right.pos.x    = (int)(i * item_width_ + item_width_);
        quad.top_right.pos.y    = (int)(item_height_);
        quad.top_right.pos.z    = 0.0f;

        Color4B c(color_, opacity_);
        quad.top_left.color     = c;
        quad.top_right.color    = c;
        quad.bottom_left.color  = c;
        quad.bottom_right.color = c;

        texture_atlas_->UpdateQuad(quad, i);
    }
}

void LabelAtlas::Render() {
    AtlasNode::Render();

#if KAWAII_DEBUG
    vec2 size = content_size();
    vec2 vertices[4] = {
        vec2(0, 0), vec2(size.x, 0), vec2(size.x, size.y), vec2(0, size.y)
    };
    DrawPoly(vertices, 4, true);
#endif
}

// LabelInterface

void LabelAtlas::set_text(const string& text) {
    if (text == text_) {
        return;
    }

    size_t len = text.length();
    if (len > texture_atlas_->capacity()) {
        texture_atlas_->ResizeCapacity(len);
    }

    text_ = text;

    UpdateAtlasValues();

    set_content_size(len * item_width_, item_height_);
    set_quads_to_draw(len);
}

}
