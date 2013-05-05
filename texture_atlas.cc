#include "kawaii/texture_atlas.h"

#include "kawaii/texture_cache.h"
#include "kawaii/logger.h"

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
    total_quads_ = 0;
    texture_ = texture;

    if (quads_ != NULL || indices_ != NULL) {
        IIERROR("TextureAtlas re-initialization is not allowd");
        return false;
    }

    quads_   = static_cast<Quad_P3F_C4B_T2F *>(calloc(sizeof(quads_[0]) * capacity_, 1));
    indices_ = static_cast<GLushort *>(calloc(sizeof(indices_[0]) * capacity_, 1));
    if (!quads_ || !indices_) {
        IIERROR("TextureAtlas not enough memory");
        if (quads_) {
            free(quads_);
        }
        if (indices_) {
            free(indices_);
        }
    }

    SetupIndices();
    SetupVboAndVao();
    SetupVbo();

    dirty_ = true;

    return true;
}

void TextureAtlas::SetupIndices() {
}

void TextureAtlas::SetupVboAndVao() {
}

void TextureAtlas::SetupVbo() {
}

void TextureAtlas::MapBuffers() {
}

}

