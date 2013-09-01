#include "label_ttf.h"

namespace sometuke {

bool LabelTTF::InitWithText(const string& text,
                            const string& font_name,
                            flaot font_size) {
    return InitWithText(text, font_name, font_size, vec2(0, 0));
}


bool LabelTTF::InitWithText(const string& text,
                            const string& font_name,
                            flaot font_size,
                            const vec2& dimentions,
                            TextHAlignment h_alignment
                            TextVAlignment v_alignment) {
    font_name_   = font_name;
    font_size    = font_size;
    dimentions_  = dimentions;
    h_alignment_ = h_alignment;
    v_alignment_ = v_alignment;

    set_text(text);

    return true;
}

void LabelTTF::set_text(const string& text) {
    if (text_.compare(text)) {
        text_ = text;
        UpdateTexture();
    }
}

bool LabelTTF::UpdateTexture() {
    FontDefinition font_def = CreateFontDef(true);
}

FontDefinition LabelTTF::CreateFontDef(bool adjust_for_resolution) {
    FontDefinition font_def;

    float content_scale_factor = Director::Instance().content_scale_factor;
    if (adjust_for_resolution) {
        font_def.font_size = font_size * content_scale_factor;
    } else {
        font_def.font_size = font_size;
    }

    font_def.font_name   = font_name_;
    font_def.h_alignment = h_alignment_;
    font_def.v_alignment = v_alignment_;

    if (adjust_for_resolution) {
        font_def.dimentions = dimentions_ * content_scale_factor;
    } else {
        font_def.dimentions = dimentions_;
    }

    // stroke
    if (stroke_enabled_) {
        font_def.stroke.stroke_enabled = true;
        font_def.stroke.stroke_color   = stroke_color_;
        font_def.stroke.stroke_size    = (adjust_for_resolution ?
                                          stroke_size_ * content_scale_factor :
                                          stroke_size_);
    } else {
        font_def.stroke.stroke_enabled = false;
    }

    // shadow
    if (shadow_enabled_) {
        font_def.shadow.shadow_enabled = true;
        font_def.shadow.shadow_blur    = shadow_blur_;
        font_def.shadow.shadow_opacity = shadow_opacity_;
        font_def.shadow.shadow_offset  = (adjust_for_resolution ?
                                          shadow_offset_ * content_scale_factor :
                                          shadow_offset_);
    } else {
        font_def.shadow.shadow_enabled = false;
    }

    font_def.font_fill_color = text_fill_color_;

    return font_def;
}


}

}
