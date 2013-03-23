#include "texture_cache.h"

#include "texture_2d.h"
#include "hashed_string.h"
#include "application.h"
#include "assets.h"

namespace kawaii {

shared_ptr<Texture2D> TextureCache::FetchFromPath(const string& path) {
    unsigned int texture_id = HashedString(path).id();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Application::Instance().assets().ReadTexture(path);
        textures_[texture_id] = texture;
    }

    return textures_[texture_id];
}

}
