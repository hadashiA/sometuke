#ifndef __sometuke__texture_atlas__
#define __sometuke__texture_atlas__

#ifndef II_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
#define II_TEXTURE_ATLAS_USE_TRIANGLE_STRIP 0
#endif

#ifndef II_TEXTURE_ATLAS_USE_VAO
#define II_TEXTURE_ATLAS_USE_VAO 0
#endif

#include "sometuke/vertices.h"

#include <memory>
#include <string>
#include <vector>

namespace sometuke {
using namespace std;
    
class Texture2D;

class TextureAtlas {
public:
    typedef vector<P3F_C4B_T2F_Quad> Quads;
    typedef vector<GLushort> Indices;

    TextureAtlas()
        : dirty_(true) {
    }

    ~TextureAtlas() {
        glDeleteBuffers(2, buffers_vbo_);
    }

    bool InitWithFile(const string& path, size_t capacity);
    bool InitWithTexture(shared_ptr<Texture2D> texture, size_t capacity);

    size_t quads_size() const {
        return quads_.size();
    }

    size_t capacity() const {
        return capacity_;
    }

    shared_ptr<Texture2D> texture() const {
        return texture_;
    }

    void set_texture(shared_ptr<Texture2D> value) {
        texture_ = value;
    }

    void ResizeCapacity(size_t new_capacity);

    void UpdateQuad(const P3F_C4B_T2F_Quad& quad, size_t index) {
        quads_.insert(quads_.begin() + index, quad);
        dirty_ = true;
    }

    void RenderQuads();
    void RenderQuads(size_t n);
    void RenderQuads(size_t n, size_t start);

private:
    void SetupIndices();
    void MapBuffers();
    void SetupVbo();

    size_t capacity_;
    Quads quads_;
    Indices indices_;
    shared_ptr<Texture2D> texture_;

    GLuint buffers_vbo_[2];      // 0:vertex 1:indices
    bool dirty_;

    GLuint vao_name_;
};

}

#endif /* defined(__sometuke__texture_atlas__) */
