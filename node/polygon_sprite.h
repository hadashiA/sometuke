#ifndef __sometuke__node_polygon_sprite__
#define __sometuke__node_polygon_sprite__

#include "sometuke/vertices.h"
#include "sometuke/node/node.h"
#include "sometuke/shader_cache.h"
#include "sometuke/texture_2d.h"
#include "OpenGL_Internal.h"

#include <OpenGLES/ES2/gl.h>

#include <functional>
#include <vector>
#include <memory>

using namespace std;

namespace sometuke {

struct SimpleTriangulator : public unary_function<const vector<vec2gl>&, vector<vec2gl> > {
public:
    vector<vec2gl> operator()(const vector<vec2gl>& vertices);
    
private:
    float Area(const vector<vec2gl>& vertices);
    bool InsideTriangle(const vec2gl& a,
                        const vec2gl& b,
                        const vec2gl& c,
                        const vec2gl& p);
    bool Snip(const vector<vec2gl>& vertices,
              const vector<size_t>& indices,
              int u, int v, int w, int n);
};

template<class Triangulator>
class PolygonSpriteBase : public Node, public TextureInterface {
public:
    PolygonSpriteBase()
        : texture_rect_rotated_(false) {
    }

    virtual ~PolygonSpriteBase() {}

    virtual shared_ptr<Texture2D> texture() const {
        return texture_;
    }

    virtual void set_texture(const shared_ptr<Texture2D>& texture) {
        texture_ = texture;
        texture_->set_tex_parameters(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
        
        UpdateBlendFunc();
    }

    bool InitWithTexture(const shared_ptr<Texture2D>& texture,
                         const vector<vec2gl>& vertices,
                         const Rect& rect, bool rotated) {
        set_vertices(vertices);
        set_texture(texture);
        set_texture_rect(rect, rotated);
        
        shader_program_ = ShaderCache::Instance().get(kShader_PositionTexture);
        
        return true;
    }


    bool InitWithTexture(const shared_ptr<Texture2D>& texture,
                         const vector<vec2gl>& vertices) {
        return InitWithTexture(texture, vertices, Rect(vec2(0, 0), texture->content_size()), false);
    }

    bool InitWithSpriteFrame(const shared_ptr<SpriteFrame>& frame,
                             const vector<vec2gl>& vertices) {
        return InitWithTexture(frame->texture, vertices, frame->rect, frame->rotated);
    }

    void set_vertices(const vector<vec2gl>& vertices) {
        area_triangle_points_ = triangulator_(vertices);
        UpdateBlendFunc();
        CalculateTexCoords();
    }

    void set_texture_rect(const Rect& rect, bool rotated) {
        texture_rect_ = rect;
        texture_rect_rotated_ = rotated;
        CalculateTexCoords();
    }

    SpriteFrame display_frame() const {
        float content_scale_factor = Director::Instance().content_scale_factor();
        return SpriteFrame(texture_,
                           texture_rect_ * content_scale_factor,
                           texture_rect_rotated_,
                           vec2(0, 0),
                           content_size() * content_scale_factor
                           );
    }

    void Render() {
        shader_program_->Use();
        shader_program_->SetUniformsForBuiltins();
        
        // set blending
        if (blend_func_src_ == GL_ONE && blend_func_dst_ == GL_ZERO) {
            glDisable(GL_BLEND);
        } else {
            glEnable(GL_BLEND);
            glBlendFunc(blend_func_src_, blend_func_dst_);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->id());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glEnableVertexAttribArray(kVertexAttrib_Position);
        glDisableVertexAttribArray(kVertexAttrib_Color);
        glEnableVertexAttribArray(kVertexAttrib_TexCoords);

        glVertexAttribPointer(kVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0,
                              area_triangle_points_.data());
        glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0,
                              tex_coords_.data());
        glDrawArrays(GL_TRIANGLES, 0, area_triangle_points_.size());
        CHECK_GL_ERROR_DEBUG();
    }

private:
    void CalculateTexCoords() {
        if (!texture_) return;

        tex_coords_.clear();
        
        float content_scale_factor = Director::Instance().content_scale_factor();
        GLfloat atlas_width  = texture_->pixel_size().x * content_scale_factor;
        GLfloat atlas_height = texture_->pixel_size().y * content_scale_factor;
        
        for (vector<vec2gl>::iterator i = area_triangle_points_.begin();
             i != area_triangle_points_.end(); ++i) {
            const vec2gl& vertex = *i;
            float u, v;
            if (texture_rect_rotated_) {
                u = (vertex.y + texture_rect_.pos.x) / atlas_width;
                v = (vertex.x + (atlas_height - texture_rect_.pos.y))  / atlas_height;
            } else {
                u = (vertex.x + texture_rect_.pos.x) / atlas_width;
                v = (vertex.y + (atlas_height - texture_rect_.pos.y))  / atlas_height;
            }
            tex_coords_.push_back(vec2gl(u, v));
        }
    }
    
    void UpdateBlendFunc() {
        if (!texture_ || !texture_->has_premultiplied_alpha()) {
            blend_func_src_ = GL_ONE;
            blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        } else {
            blend_func_src_ = GL_SRC_ALPHA;
            blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        }
    }
    
    shared_ptr<Texture2D> texture_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    Triangulator triangulator_;

    vector<vec2gl> area_triangle_points_;
    vector<vec2gl> tex_coords_;

    shared_ptr<GLProgram> shader_program_;

    Rect texture_rect_;
    bool texture_rect_rotated_;
};

class PolygonSprite : public PolygonSpriteBase<SimpleTriangulator> {
};


}

#endif /* defined(__sometuke__node_polygon_sprite__) */
