#include "kawaii/texture_atlas.h"

#include "kawaii/texture_cache.h"
#include "kawaii/logger.h"
#include "kawaii/texture_2d.h"

#include "OpenGL_Internal.h"

#include <cassert>
#include <cstdlib>

namespace kawaii {

bool TextureAtlas::InitWithFile(const string& path, size_t capacity) {
    shared_ptr<Texture2D> texture = TextureCache::Instance().FetchFromPath(path);
    if (!texture) {
        IIERROR("could not read texture file %s", path.c_str());
        return false;
    }
    return InitWithTexture(texture, capacity);
}

bool TextureAtlas::InitWithTexture(shared_ptr<Texture2D> texture, size_t capacity) {
    capacity_ = capacity;
    texture_  = texture;

    quads_.resize(capacity);
    indices_.resize(capacity * 6);

    SetupIndices();
    SetupVbo();

    dirty_ = true;

    return true;
}

void TextureAtlas::ResizeCapacity(size_t new_capacity) {
    if (new_capacity == capacity_) {
        return;
    }

    if (new_capacity < quads_.capacity()) {
        Quads::iterator last = quads_.begin() + new_capacity;
        quads_.erase(last, quads_.end());
    }
    if (new_capacity * 6 < indices_.capacity()) {
        Indices::iterator last = indices_.begin() + (new_capacity * 6);
        indices_.erase(last, indices_.end());
    }

    quads_.resize(new_capacity);
    indices_.resize(new_capacity * 6);

    SetupIndices();
    MapBuffers();

    dirty_ = true;
}

void TextureAtlas::SetupIndices() {
    for(int i = 0; i < capacity_; i++) {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * capacity_, quads_.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_[0]) * capacity_ * 6, indices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TextureAtlas::RenderQuads() {
    RenderQuads(quads_.size(), 0);
}

void TextureAtlas::RenderQuads(size_t n) {
    RenderQuads(n, 0);
}

void TextureAtlas::RenderQuads(size_t n, size_t start) {
    glBindTexture(GL_TEXTURE_2D, texture_->id());

#define kQuadSize sizeof(quads_[0].bl)
    glBindBuffer(GL_ARRAY_BUFFER, buffers_vbo_[0]);
    if (dirty_) {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * start, sizeof(quads_[0]) * n, &quads_[start]);
        dirty_ = false;
    }

    // attributes
    glEnableVertexAttribArray(kVertexAttrib_Position);
    glEnableVertexAttribArray(kVertexAttrib_Color);
    glEnableVertexAttribArray(kVertexAttrib_TexCoords);

#define QUAD_SIZE sizeof(quads_[0].bottom_left)
    // position
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, QUAD_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, pos));

    // color
    glVertexAttribPointer(kVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, QUAD_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, color));

    // texCoord
    glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, QUAD_SIZE,
                          (GLvoid *)offsetof(P3F_C4B_T2F, tex_coord));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_vbo_[1]);
#if II_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP,
                   (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid *)(start * 6 * sizeof(indices_[0])));
    
#else
    glDrawElements(GL_TRIANGLES,
                   (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid *)(start*6*sizeof(indices_[0])));
#endif

    CHECK_GL_ERROR_DEBUG();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(kVertexAttrib_Position);
    glDisableVertexAttribArray(kVertexAttrib_Color);
    glDisableVertexAttribArray(kVertexAttrib_TexCoords);
}

}
