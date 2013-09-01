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
          dimentions_(0, 0),
          is_texture_dirty_(false),
          h_alignment(TextAlignment::LEFT),
          v_alignment(TextAlignment::TOP)
    {
    }
                     
    virtual ~LabelTTF() {}

    bool InitWithText(const string& text,
                      const string& font_name,
                      float font_size);

    bool InitWithText(const string& text,
                      const string& font_name,
                      flaot font_size,
                      const vec2& dimentions,
                      TextHAlignment h_alignment = TextHAlignment::LEFT,
                      TextVAlignment v_alignment = TextVAlignment::TOP);

    bool initWithStringAndTextDefinition(const string& string,
                                         const FontDefinition& font_definition);

    bool LabelTTF::UpdateTexture();

    void set_text(const string& text);

    const string& text() const {
        return text_;
    }

    TextHAlignment h_alignment() const {
        return h_alignment_;
    }

    void set_h_alignment(TextHAlignment h_alignment) {
        h_alignment_ = h_alignment;
    }

private:
    FontDefinition CreateFontDef(bool adjust_for_resolution);

    string text_;
    vec2 dimentions_;
    bool is_texture_dirty_;

    TextHAlignment h_alignment_;
    TextVAlignment v_alignment_;
    string font_name;
    float font_size;

    // font shadow
    bool shadow_enabled_;
    float shadow_offset_;
    float shadow_opacity_;
    float shadwo_blur_;

    // font stroke
    bool stroke_enabled_;
    Color3B stroke_color_;
    float stroke_size_;
    Color3B text_fill_color_;
};
    
}

#endif /* defined(__Hitasura__label_ttf__) */
