#include "kawaii/texture_cache.h"

#include "kawaii/texture_2d.h"
#include "kawaii/hashed_string.h"
#include "kawaii/application.h"
#include "kawaii/assets_loader.h"

namespace kawaii {

shared_ptr<Texture2D> TextureCache::FetchFromPath(const string& path) {
    unsigned int texture_id = HashedString(path).id();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Application::Instance().loader().ReadTexture(path);
        textures_[texture_id] = texture;
    }

    return textures_[texture_id];
}

}
