#ifndef __kawaii__sprite__
#define __kawaii__sprite__

#include "vertices.h"
#include "node.h"

#include <memory>
#include <string>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {

class Texture2D;
class GLProgram;

class Sprite : Node {
public:
    Sprite();
    virtual ~Sprite() {}

    virtual void Render();

    const bvec2& fliped() const {
        return fliped_;
    }

    const GLubyte blend_func_src() const {
        return blend_func_src_;
    }

    const GLubyte blend_func_dst() const {
        return blend_func_dst_;
    }

    shared_ptr<Texture2D> texture() const {
        return texture_;
    }

    const Color3B color() const {
        return (does_opacity_modify_rgb_ ? color_unmodified_ : color_);
    }

    const GLubyte opacity() const {
        return opacity_;
    }

    bool does_opacity_modify_rgb() const {
        return does_opacity_modify_rgb_;
    }

    void set_fliped_x(bool value) {
        fliped_.x = value;
    }

    void set_fliped_y(bool value) {
        fliped_.y = value;
    }

    void set_blend_func_src(GLubyte value) {
        blend_func_src_ = value;
    }

    void set_blend_func_dst(GLubyte value) {
        blend_func_dst_ = value;
    }

    void set_color(const Color3B& value);
    void set_opacity(GLubyte value);
    void set_opacity_modify_rgb(bool value);
    void set_texture(shared_ptr<Texture2D> value);
    void set_texture_rect(const Rect& rect);
    void set_texture_rect(const Rect& rect, bool rotated,
                          const vec2& untrimmed_size);

private:
    void UpdateQuadColor();
    void UpdateQuadTexCoords();

    shared_ptr<Texture2D> texture_;
    shared_ptr<GLProgram> shader_program_;

    bvec2 fliped_;

    // renderer stuff
    Quad_P3F_C4B_T2F quad_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    // color stuff
    GLubyte opacity_;
    Color3B color_;
    Color3B color_unmodified_;
    bool does_opacity_modify_rgb_;

    // vertex stuff
    Rect vertex_rect_;
};

}

#endif /* defined(__kawaii__sprite__) */