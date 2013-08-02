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

struct Triangulator : public unary_function<const vector<vec2gl>&, vector<vec2gl> > {
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

class PolygonSprite : public Node, public TextureInterface {
public:
    PolygonSprite()
        : texture_rect_rotated_(false),
          texture_rect_(0, 0, 0, 0) {
    }

    virtual ~PolygonSprite() {}

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
        
        shader_program_ = ShaderCache::Instance().get(kShader_PositionTexture_uColor);
        
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

    void Render();

private:
    void CalculateTexCoords();     

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

}

#endif /* defined(__sometuke__node_polygon_sprite__) */
