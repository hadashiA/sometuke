#ifndef __Hitasura__label_ttf__
#define __Hitasura__label_ttf__

#include "sometuke/node/sprite.h"
#include "sometuke/application_component.h"

namespace sometuke {

class LabelTTF : public Sprite,
                 public LabelInterface {
public:
    LabelTTF()
        : is_texture_dirty_(false) {
    }
                     
    virtual ~LabelTTF() {}

    bool InitWithText(const string& text,
                      const string& font_name,
                      float font_size);

    bool InitWithText(const string& text,
                      const string& font_name,
                      float font_size,
                      vec2 dimentions);
    

    void set_text(const string& text);

    const string& text() const {
        return text_;
    }

private:
    string text_;
    string font_name_;
    float fond_size_;
    Color4B font_color_;
    vec2 dimentions;
    bool is_texture_dirty_;

    // alignment
    SystemFontLoader::TextAlignment horizontal_alignment_;
    SystemFontLoader::VerticalTextAlignment vertical_alignment_;

    // shadow
    Color4B shadow_color_;
    vec2 shadow_offset_;
    float shasow_blur_radius_;

    // outline
    Color4B outline_color_;
    float outline_width_;

    // font adjustments
    bool adjusts_font_size_to_fit_;
    float minimum_font_size_;
    float baseline_adjustment_;
};
    
}

#endif /* defined(__Hitasura__label_ttf__) */
