#ifndef __skidarake__node_filled_polygon__
#define __skidarake__node_filled_polygon__

#include "skidarake/vertices.h"
#include "skidarake/node/node.h"
#include "skidarake/shader_cache.h"
#include "skidarake/texture_2d.h"

#include <OpenGLES/ES2/gl.h>

#include <functional>
#include <vector>
#include <memory>

using namespace std;

namespace skidarake {

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

template<class Triangulator = SimpleTriangulator>
class FilledPolygon : public Node, public TextureInterface {
public:
    virtual ~FilledPolygon() {}

    
    void Render() {
        shader_program_->Use();
        shader_program_->SetUniformsForBuiltins();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->id());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // set blending
        if (blend_func_src_ == GL_ONE && blend_func_dst_ == GL_ZERO) {
            glDisable(GL_BLEND);
        } else {
            glEnable(GL_BLEND);
            glBlendFunc(blend_func_src_, blend_func_dst_);
        }

        glEnableVertexAttribArray(kVertexAttrib_Position);
        glEnableVertexAttribArray(kVertexAttrib_TexCoords);

        glVertexAttribPointer(kVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0,
                              area_triangle_points_.data());
        glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0,
                              tex_coords_.data());
        glDrawArrays(GL_TRIANGLES, 0, area_triangle_points_.size());
    
        glDisableVertexAttribArray(kVertexAttrib_Position);
        glDisableVertexAttribArray(kVertexAttrib_TexCoords);
    }


    virtual shared_ptr<Texture2D> texture() const {
        return texture_;
    }

    virtual void set_texture(const shared_ptr<Texture2D>& texture) {
        texture_ = texture;
        texture_->set_tex_parameters(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
        
        UpdateBlendFunc();
        CalculateTexCoords();
    }

    bool InitWithVertices(const vector<vec2gl>& vertices,
                          const shared_ptr<Texture2D>& texture) {
        set_vertices(vertices);
        set_texture(texture);
        
        shader_program_ = ShaderCache::Instance().get(kShader_PositionTexture_uColor);
        
        return true;
    }

    void set_vertices(const vector<vec2gl>& vertices) {
        area_triangle_points_ = triangulator_(vertices);
        CalculateTexCoords();
    }

private:
    void CalculateTexCoords() {
        if (!texture_) return;

        tex_coords_.clear();
        
        float content_scale_factor = Application::Instance().content_scale_factor();
        GLfloat scale = 1.0f / texture_->pixel_size().x * content_scale_factor;
        
        for (vector<vec2gl>::iterator i = area_triangle_points_.begin();
             i != area_triangle_points_.end(); ++i) {
            const vec2gl& vertex = *i;
            vec2gl tex_coord(vertex.x * scale, 1 - (vertex.y * scale));
            tex_coords_.push_back(tex_coord);
        }
    }
    
    void UpdateBlendFunc() {
        if (!texture_ || !texture_->has_premultipled_alpha()) {
            blend_func_src_ = GL_SRC_ALPHA;
            blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
        } else {
            blend_func_src_ = GL_ONE;
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
};

}

#endif /* defined(__skidarake__node_filled_polygon__) */
