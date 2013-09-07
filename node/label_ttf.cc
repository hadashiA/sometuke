#include "sometuke/node/label_ttf.h"

#include "sometuke/texture_2d.h"

namespace sometuke {

bool LabelTTF::InitWithText(const string& text,
                            const string& font_name,
                            float font_size) {
    return InitWithText(text, font_name, font_size, vec2(0, 0));
}


bool LabelTTF::InitWithText(const string& text,
                            const string& font_name,
                            float font_size,
                            const vec2& dimensions,
                            TextHAlignment h_alignment,
                            TextVAlignment v_alignment) {
    font_name_       = font_name;
    font_size_       = font_size;
    dimensions_      = dimensions;
    h_alignment_     = h_alignment;
    v_alignment_     = v_alignment;
    text_fill_color_ = Color3B::WHITE;
    
    set_text(text);

    return true;
}

bool LabelTTF::InitWithText(const string& text, const FontDefinition& definition) {
    font_name_   = definition.name;
    dimensions_  = definition.dimensions;
    h_alignment_ = definition.h_alignment;
    v_alignment_ = definition.v_alignment;
    font_size_   = definition.size;
    text_fill_color_  = definition.fill_color;

    shadow_enabled_ = definition.shadow.enabled;
    if (shadow_enabled_) {
        shadow_opacity_ = 0.5;
        shadow_blur_    = definition.shadow.blur;
    }

    stroke_enabled_ = definition.stroke.enabled;
    if (stroke_enabled_) {
        stroke_color_ = definition.stroke.color;
        stroke_size_  = definition.stroke.size;
    }

    set_text(text);
    return true;
}

void LabelTTF::set_text(const string& text) {
    if (text_ != text) {
        text_ = text;
        UpdateTexture();
    }
}

void LabelTTF::set_font_name(const string& font_name) {
    if (font_name_ != font_name) {
        font_name_ = font_name;
        if (!text_.empty()) {
            UpdateTexture();
        }
    }
}

void LabelTTF::set_font_size(const float font_size) {
    if (font_size_ != font_size) {
        font_size_ = font_size;
        if (!text_.empty()) {
            UpdateTexture();
        }
    }
}

void LabelTTF::set_dimensions(const vec2& dimensions) {
    if (dimensions_ != dimensions) {
        dimensions_ = dimensions;
        if (!text_.empty()) {
            UpdateTexture();
        }
    }
}

void LabelTTF::set_h_alignment(TextHAlignment h_alignment) {
    if (h_alignment_ != h_alignment) {
        h_alignment_ = h_alignment;
        if (!text_.empty()) {
            UpdateTexture();
        }
    }
}

void LabelTTF::set_v_alignment(TextVAlignment v_alignment) {
    if (v_alignment_ != v_alignment) {
        v_alignment_ = v_alignment;
                if (!text_.empty()) {
            UpdateTexture();
        }
    }
}

bool LabelTTF::UpdateTexture() {
    FontDefinition font_def = CreateFontDefinition(true);
    shared_ptr<Image> image = Director::Instance().image().Create();
    if (!image->InitWithText(text_, font_def)) {
        return false;
    }

    shared_ptr<Texture2D> texture = make_shared<Texture2D>();
    if (!texture->InitWithImage(image)) {
        return false;
    }

    // iPad ?
    // if( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad ) {
    //     if( CC_CONTENT_SCALE_FACTOR() == 2 )
    //         [tex setResolutionType:kCCResolutioniPadRetinaDisplay];
    //     else
    //         [tex setResolutionType:kCCResolutioniPad];
    // // iPhone ?
    // } else {
    //     if( CC_CONTENT_SCALE_FACTOR() == 2 )
    //         [tex setResolutionType:kCCResolutioniPhoneRetinaDisplay];
    //     else
    //         [tex setResolutionType:kCCResolutioniPhone];
    // }

    set_texture(texture);
    set_texture_rect(Rect(0, 0, texture->content_size().x, texture->content_size().y));
    return true;
}

FontDefinition LabelTTF::CreateFontDefinition(bool adjust_for_resolution) {
    FontDefinition definition;

    float content_scale_factor = Director::Instance().content_scale_factor();
    definition.size = (adjust_for_resolution ? font_size_ * content_scale_factor : font_size_);

    definition.name        = font_name_;
    definition.h_alignment = h_alignment_;
    definition.v_alignment = v_alignment_;
    definition.fill_color  = text_fill_color_;

    // stroke
    definition.stroke.enabled = stroke_enabled_;
    if (stroke_enabled_) {
        definition.stroke.color = stroke_color_;
        definition.stroke.size  = (adjust_for_resolution ?
                                   stroke_size_ * content_scale_factor :
                                   stroke_size_);
    }
        
    // shadow
    definition.shadow.enabled = shadow_enabled_;
    if (shadow_enabled_) {
        definition.shadow.blur = shadow_blur_;
        definition.shadow.offset = (adjust_for_resolution ?
                                    shadow_offset_ * content_scale_factor:
                                    shadow_offset_);
    }
    return definition;
}

}
