#ifndef __kawaii__texture_atlas__
#define __kawaii__texture_atlas__

#include "kawaii/vertices.h"

#include <memory>
#include <string>

namespace kawaii {
using namespace std;
    
class Texture2D;

class TextureAtlas {
public:
    TextureAtlas()
        : total_quads_(0),
          capacity_(0),
          dirty_(true) {
    }

    bool InitWithFile(const string& path, size_t capacity);
    bool InitWithTexture(shared_ptr<Texture2D> texture, size_t capacity);

    size_t total_quads() const {
        return total_quads_;
    }

    size_t capacity() const {
        return capacity_;
    }

    shared_ptr<Texture2D> texture() const {
        return texture_;
    }

private:
    void SetupIndices();
    void SetupVboAndVao();
    void SetupVbo();
    void MapBuffers();

    size_t total_quads_;
    size_t capacity_;
    Quad_P3F_C4B_T2F *quads_;
    GLushort *indices_;
    shared_ptr<Texture2D> texture_;

    GLuint buffers_vbo[2];      // 0:vertex 1:indices
    bool dirty_;

    GLuint vao_name_;
};

}

#endif /* defined(__kawaii__texture_atlas__) */
