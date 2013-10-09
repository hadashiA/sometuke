#include "sometuke/texture_atlas.h"

#include "sometuke/texture_cache.h"
#include "sometuke/logger.h"
#include "sometuke/texture_2d.h"

#include "OpenGL_Internal.h"

#include <cassert>
#include <cstdlib>

namespace sometuke {

bool TextureAtlas::InitWithFile(const string& path, size_t capacity) {
    shared_ptr<Texture2D> texture = TextureCache::Instance().FetchFromPath(path);
    if (!texture) {
        S2ERROR("could not read texture file %s", path.c_str());
        return false;
    }
    return InitWithTexture(texture, capacity);
}

bool TextureAtlas::InitWithTexture(shared_ptr<Texture2D> texture, size_t capacity) {
    texture_  = texture;

    quads_.reserve(capacity);
    indices_.reserve(capacity * 6);

    SetupIndices();
    SetupVbo();

    dirty_ = true;

    return true;
}

void TextureAtlas::Resize(size_t new_size) {
    if (new_size <= size_) {
        return;
    }

    if (new_size < quads_.size()) {
        Quads::iterator last = quads_.begin() + new_size;
        quads_.erase(last, quads_.end());
    }
    if (new_size * 6 < indices_.size()) {
        Indices::iterator last = indices_.begin() + (new_size * 6);
        indices_.erase(last, indices_.end());
    }

    quads_.resize(new_size);
    indices_.resize(new_size * 6);

    size_ = new_size;
    SetupIndices();
    MapBuffers();

    dirty_ = true;
}

void TextureAtlas::SetupIndices() {
    for(int i = 0; i < size_; i++) {
#if II_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        indices_[i*6+0] = i*4+0;
        indices_[i*6+1] = i*4+0;
        indices_[i*6+2] = i*4+2;
        indices_[i*6+3] = i*4+1;
        indices_[i*6+4] = i*4+3;
        indices_[i*6+5] = i*4+3;
#else
        indices_[i*6+0] = i*4+0;
        indices_[i*6+1] = i*4+1;
        indices_[i*6+2] = i*4+2;
	
        // inverted index. issue #179
        indices_[i*6+3] = i*4+3;
        indices_[i*6+4] = i*4+2;
        indices_[i*6+5] = i*4+1;
#endif
    }
}

void TextureAtlas::SetupVbo() {
    glGenBuffers(2, &buffers_vbo_[0]);
    MapBuffers();
}

void TextureAtlas::MapBuffers() {
    // glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers_vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * size_,
                 &quads_[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_[0]) * size_ * 6,
                 &indices_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

void TextureAtlas::RenderQuads() {
    RenderQuads(size_, 0);
}

void TextureAtlas::RenderQuads(size_t n) {
    RenderQuads(n, 0);
}

void TextureAtlas::RenderQuads(size_t n, size_t start) {
    glBindTexture(GL_TEXTURE_2D, texture_->id());

#define VERTEX_SIZE sizeof(quads_[0].bl)
    glBindBuffer(GL_ARRAY_BUFFER, buffers_vbo_[0]);
    if (dirty_) {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * start, sizeof(quads_[0]) * n,
                        &quads_[start]);
        CHECK_GL_ERROR_DEBUG();
        dirty_ = false;
    }

    // attributes
    glEnableVertexAttribArray(kVertexAttrib_Position);
    glEnableVertexAttribArray(kVertexAttrib_Color);
    glEnableVertexAttribArray(kVertexAttrib_TexCoords);

    // position
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, pos));

    // color
    glVertexAttribPointer(kVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, color));

    // texCoord
    glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, tex_coord));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR_DEBUG();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_vbo_[1]);
#if II_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP,
                   (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid *)(start * 6 * sizeof(indices_[0])));
    
#else
    glDrawElements(GL_TRIANGLES,
                   (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid *)(start * 6 * sizeof(indices_[0])));
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
 

    glDisableVertexAttribArray(kVertexAttrib_Position);
    glDisableVertexAttribArray(kVertexAttrib_Color);
    glDisableVertexAttribArray(kVertexAttrib_TexCoords);
}

}
