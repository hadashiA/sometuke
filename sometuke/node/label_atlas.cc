#include "sometuke/node/label_atlas.h"

#include "sometuke/drawing_primitives.h"

namespace sometuke {

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
    P3F_C4B_T2F_Quad quad;
    const char *s = text_.c_str();

    shared_ptr<Texture2D> texture = texture_atlas_->texture();
    vec2 pixel_size = texture->pixel_size();

    float content_scale_factor = Director::Instance().content_scale_factor();
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

        quad.tl.tex_coord.u = left;
        quad.tl.tex_coord.v = top;
        quad.tr.tex_coord.u = right;
        quad.tr.tex_coord.v = top;
        quad.bl.tex_coord.u = left;
        quad.bl.tex_coord.v = bottom;
        quad.br.tex_coord.u = right;
        quad.br.tex_coord.v = bottom;

        quad.bl.pos.x = (int)(i * item_width_);
        quad.bl.pos.y = 0;
        quad.bl.pos.z = 0.0f;
        quad.br.pos.x = (int)(i * item_width_ + item_width_);
        quad.br.pos.y = 0;
        quad.br.pos.z = 0.0f;
        quad.tl.pos.x = (int)(i * item_width_);
        quad.tl.pos.y = (int)(item_height_);
        quad.tl.pos.z = 0.0f;
        quad.tr.pos.x = (int)(i * item_width_ + item_width_);
        quad.tr.pos.y = (int)(item_height_);
        quad.tr.pos.z = 0.0f;

        Color4B c(color_, opacity_);
        quad.tl.color = c;
        quad.tr.color = c;
        quad.bl.color = c;
        quad.br.color = c;

        texture_atlas_->UpdateQuad(quad, i);
    }
}

void LabelAtlas::Render() {
    AtlasNode::Render();

#if KAWAII_DEBUG
    if (Director::Instance().director().debug_drawing()) {
        vec2 size = content_size();
        vec2 vertices[4] = {
            vec2(0, 0), vec2(size.x, 0), vec2(size.x, size.y), vec2(0, size.y)
        };
        DrawPoly(vertices, 4, true);
    }
#endif
}

// LabelInterface

void LabelAtlas::set_text(const string& text) {
    if (text == text_) {
        return;
    }

    size_t len = text.length();
    if (len > texture_atlas_->size()) {
        texture_atlas_->Resize(len);
    }

    text_ = text;

    UpdateAtlasValues();

    set_content_size(len * item_width_, item_height_);
    set_quads_to_draw(len);
}

}
