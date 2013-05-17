#ifndef __kawaii__node_atlas_node__
#define __kawaii__node_atlas_node__

#include <string>
#include <memory>

#include "kawaii/node/node.h"
#include "kawaii/color.h"
#include "kawaii/texture_2d.h"
#include "kawaii/texture_atlas.h"
#include "kawaii/gl_program.h"

namespace kawaii {

class AtlasNode : public Node, public TextureInterface, public RGBAInterface {
public:
    AtlasNode()
        : items_per_row_(0),
          items_per_column_(0),
          item_width_(0),
          item_height_(0),
          opacity_(255),
          color_(255, 255, 255),
          opacity_modify_rgb_(true),
          blend_func_src_(GL_ONE),
          blend_func_dst_(GL_ONE_MINUS_SRC_ALPHA) {
    }

    bool InitWithTileFile(const string& path,
                          size_t tile_width, size_t tile_height,
                          size_t items_to_render);

    virtual void Render();

    void set_quads_to_draw(size_t value) {
        quads_to_draw_ = value;
    }

    // RGBAInterface
    virtual const Color3B color() const {
        return color_;
    }

    virtual const GLubyte opacity() const {
        return opacity_;
    }

    virtual bool does_opacity_modify_rgb() const {
        return opacity_modify_rgb_;
    }

    virtual void set_color(const Color3B& value) {
        color_ = color_unmodified_ = value;
        if (opacity_modify_rgb_) {
            color_.r = value.r * opacity_ / 255;
            color_.g = value.g * opacity_ / 255;
            color_.b = value.b * opacity_ / 255;
        }
    }

    virtual void set_opacity(GLubyte value) {
        opacity_ = value;
        if (opacity_modify_rgb_) {
            set_color(color_unmodified_);
        }
    }

    virtual void set_opacity_modify_rgb(bool value) {
        Color3B old_color = color_;
        opacity_modify_rgb_ = value;
        set_color(old_color);
    }

    // TextureInterface

    virtual shared_ptr<Texture2D> texture() const {
        return texture_atlas_->texture();
    }

    virtual void set_texture(shared_ptr<Texture2D> value) {
        texture_atlas_->set_texture(value);
    }

    virtual void UpdateAtlasValues() = 0;

protected:
    void CalculateMaxItems() {
        vec2 size = texture_atlas_->texture()->content_size();
        items_per_column_ = size.y / item_height_;
        items_per_row_    = size.x / item_width_;
    }

    void UpdateBlendFunc() {
        if (!texture_atlas_->texture()->has_premultipled_alpha()) {
            blend_func_src_ = GL_SRC_ALPHA;
            blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        }
    }

    void UpdateOpacityModifyRGB() {
        opacity_modify_rgb_ = texture_atlas_->texture()->has_premultipled_alpha();
    }

    shared_ptr<TextureAtlas> texture_atlas_;
    size_t items_per_row_;
    size_t items_per_column_;
    size_t item_width_;
    size_t item_height_;
    size_t quads_to_draw_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    GLubyte opacity_;
    bool opacity_modify_rgb_;
    Color3B color_;
    Color3B color_unmodified_;

    GLint uniform_color_;

    shared_ptr<GLProgram> shader_program_;
};

}

#endif /* defined(__kawaii__node_atlas_node__) */
