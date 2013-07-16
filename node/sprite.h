#ifndef __skidarake__node_sprite__
#define __skidarake__node_sprite__

#include "skidarake/vertices.h"
#include "skidarake/node/node.h"
#include "skidarake/sprite_frame.h"

#include <memory>
#include <string>

#include <OpenGLES/ES2/gl.h>

namespace skidarake {

class Texture2D;
class GLProgram;
class Animation;
class Animate;

class Sprite : public Node, public TextureInterface, public RGBAInterface {
public:
    Sprite();
    virtual ~Sprite() {}
    virtual bool Init() { return true; }

    bool InitWithTextureFile(const string& path);
    bool InitWithTexture(const shared_ptr<Texture2D>& texture);
    bool InitWithTexture(const shared_ptr<Texture2D>& texture,
                         const Rect& rect, bool rotated = false);
    bool InitWithSpriteFrame(const SpriteFrame& sprite_frame);
    bool InitWithSpriteFrame(weak_ptr<SpriteFrame> sprite_frame);

    virtual void Render();

    virtual void set_texture(const shared_ptr<Texture2D>& value);

    virtual shared_ptr<Texture2D> texture() const {
        return texture_;
    }

    const bool fliped_x() const {
        return fliped_x_;
    }

    const bool fliped_y() const {
        return fliped_y_;
    }

    const GLubyte blend_func_src() const {
        return blend_func_src_;
    }

    const GLubyte blend_func_dst() const {
        return blend_func_dst_;
    }

    SpriteFrame display_frame() const {
        float content_scale_factor = Application::Instance().content_scale_factor();

        return SpriteFrame(texture_,
                           vertex_rect_ * content_scale_factor,
                           vertex_rect_rotated_,
                           unflipped_offset_position_from_center_ * content_scale_factor,
                           content_size() * content_scale_factor,
                           "");
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

    void flip_x(bool value) {
        fliped_x_ = value;
    }

    void flip_y(bool value) {
        fliped_y_ = value;
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
    void set_texture_rect(const Rect& rect);
    void set_texture_rect(const Rect& rect, bool rotated,
                          const vec2& untrimmed_size);
    void set_display_frame(const SpriteFrame& sprite_frame);
    void set_display_frame(weak_ptr<SpriteFrame> sprite_frame);

private:
    void UpdateQuadColor();
    void UpdateQuadTexCoords();

    shared_ptr<Texture2D> texture_;
    shared_ptr<GLProgram> shader_program_;

    bool fliped_x_;
    bool fliped_y_;

    // renderer stuff
    P3F_C4B_T2F_Quad quad_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    // color stuff
    GLubyte opacity_;
    Color3B color_;
    Color3B color_unmodified_;
    bool does_opacity_modify_rgb_;

    // vertex stuff
    Rect vertex_rect_;
    bool vertex_rect_rotated_;
    vec2 offset_position_;
    vec2 unflipped_offset_position_from_center_;
};

}

#endif /* defined(__skidarake__node_sprite__) */
