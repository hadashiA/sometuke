#ifndef __Hitasura__label_ttf__
#define __Hitasura__label_ttf__

#include "sometuke/node/sprite.h"
#include "sometuke/application_component.h"

namespace sometuke {

class LabelTTF : public Sprite,
                 public LabelInterface {
public:
    LabelTTF()
        : text_(""),
          dimensions_(0, 0),
          is_texture_dirty_(false),
          h_alignment_(TextHAlignment::LEFT),
          v_alignment_(TextVAlignment::TOP),
          shadow_enabled_(false),
          stroke_enabled_(false) {
    }
                     
    virtual ~LabelTTF() {}
                     
    bool InitWithText(const string& text,
                      const string& font_name,
                      float font_size);
    
    bool InitWithText(const string& text,
                      const string& font_name,
                      float font_size,
                      const vec2& dimensions,
                      TextHAlignment h_alignment = TextHAlignment::LEFT,
                      TextVAlignment v_alignment = TextVAlignment::TOP);

    bool InitWithText(const string& string,
                      const FontDefinition& definition);

    bool UpdateTexture();

    const string& text() const {
        return text_;
    }

    const string& font_name() const {
        return font_name_;
    }

    const float font_size() const {
        return font_size_;
    }

    const vec2 dimensions() const {
        return dimensions_;
    }

    TextHAlignment h_alignment() const {
        return h_alignment_;
    }

    TextVAlignment v_alignment() const {
        return v_alignment_;
    }

    void set_text(const string& text);
    void set_font_name(const string& font_name);
    void set_font_size(const float fotn_size);
    void set_dimensions(const vec2& dimensions);
    void set_h_alignment(TextHAlignment h_alignment);
    void set_v_alignment(TextVAlignment v_alignment);

private:
    FontDefinition CreateFontDefinition(bool adjust_for_resolution);

    string text_;
    vec2 dimensions_;
    bool is_texture_dirty_;

    TextHAlignment h_alignment_;
    TextVAlignment v_alignment_;
    string font_name_;
    float font_size_;

    // font shadow
    bool shadow_enabled_;
    vec2 shadow_offset_;
    float shadow_opacity_;
    float shadow_blur_;

    // font stroke
    bool stroke_enabled_;
    Color3B stroke_color_;
    float stroke_size_;
    Color3B text_fill_color_;
};
    
}

#endif /* defined(__Hitasura__label_ttf__) */
