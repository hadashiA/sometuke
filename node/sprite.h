#ifndef __sometuke__node_sprite__
#define __sometuke__node_sprite__

#include "sometuke/vertices.h"
#include "sometuke/node/node.h"
#include "sometuke/sprite_frame.h"

#include <memory>
#include <string>
#include <cassert>

#include <OpenGLES/ES2/gl.h>

namespace sometuke {

class Texture2D;
class GLProgram;
class Animation;
class Animate;
class SpriteBatchNode;

class Sprite : public Node, public TextureInterface, public RGBAInterface {
public:
    Sprite();

    virtual ~Sprite() {}
    virtual bool Init() { return true; }

    bool InitWithFilename(const string& path);
    bool InitWithTexture(const shared_ptr<Texture2D>& texture);
    bool InitWithTexture(const shared_ptr<Texture2D>& texture,
                         const Rect& rect, bool rotated = false);
    bool InitWithSpriteFrame(const SpriteFrame& sprite_frame);
    bool InitWithSpriteFrame(const shared_ptr<SpriteFrame>& sprite_frame);

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
        float content_scale_factor = Director::Instance().content_scale_factor();

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

    void flip_x(const bool value) {
        if (fliped_x_ != value) {
            fliped_x_ = value;
            set_texture_rect(vertex_rect_, vertex_rect_rotated_, content_size_);
        }
    }

    void flip_y(const bool value) {
        if (fliped_y_ != value) {
            fliped_y_ = value;
            set_texture_rect(vertex_rect_, vertex_rect_rotated_, content_size_);
        }
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
    void set_display_frame(const weak_ptr<SpriteFrame>& sprite_frame);


    void DirtyRecursively() {
        if (batch_node_ && !recursive_dirty_) {
            dirty_ = recursive_dirty_ = true;
            for (vector<shared_ptr<Node> >::iterator i = children_.begin(); i != children_.end(); ++i) {
                const shared_ptr<Sprite>& sprite = static_pointer_cast<Sprite>(*i);
                sprite->DirtyRecursively();
            }
        }
    }

    void set_position(const vec3& position) {
        Node::set_position(position);
        DirtyRecursively();
    }

    void set_rotation(const float degrees) {
        Node::set_rotation(degrees);
        DirtyRecursively();
    }

    void set_scale_x(const float scale_x) {
        Node::set_scale_x(scale_x);
        DirtyRecursively();
    }

    void set_scale_y(const float scale_y) {
        Node::set_scale_y(scale_y);
        DirtyRecursively();
    }

    void set_scale(const float scale) {
        Node::set_scale(scale);
        DirtyRecursively();
    }

    void set_skew_x(const float skew_x) {
        Node::set_skew_x(skew_x);
        DirtyRecursively();
    }

    void set_skew_y(const float skew_y) {
        Node::set_skew_y(skew_y);
        DirtyRecursively();
    }

    void set_z_order(int z_order) {
        Node::set_z_order(z_order);
        DirtyRecursively();
    }

    void set_anchor_point(const vec2& value) {
        Node::set_anchor_point(value);
        DirtyRecursively();
    }

    void set_ignore_anchor_point_for_position(bool value) {
        assert(!batch_node_);
        Node::set_ignore_anchor_point_for_position(value);
    }

    void set_visible(const bool value) {
        Node::set_visible(value);
        DirtyRecursively();
    }

    void set_batch_node(const shared_ptr<SpriteBatchNode>& value) {
        batch_node_ = value;

        if (value) {
            transform_to_batch_ = mat4::Identity();
            texture_atlas_ = value->texture_atlas();
        } else {
            texture_atlas_.reset();
            dirty_ = recursive_dirty_ = false;

            float x1 = offset_position_.x;
            float y1 = offset_position_.y;
            float x2 = x1 + rect.size.x;
            float y2 = y1 + rect.size.y;
            
            // Don't update Z
            quad_.bl.pos = vec3(x1, y1, 0);
            quad_.br.pos = vec3(x2, y1, 0);
            quad_.tl.pos = vec3(x1, y2, 0);
            quad_.tr.pos = vec3(x2, y2, 0);
        }
    }

    void UpdateTextureAtlas();

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

    // batch node stuff
    weak_ptr<SpriteBatchNode> batch_node_;
    weak_ptr<TextureAtlas> texture_atlas_;
    bool dirty_;
    bool recursive_dirty_;
    mat4 transform_to_batch_;
};

}

#endif /* defined(__sometuke__node_sprite__) */
