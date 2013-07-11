#include "skidarake/texture_cache.h"

#include "skidarake/texture_2d.h"
#include "skidarake/hashed_string.h"
#include "skidarake/application.h"
#include "skidarake/assets_loader.h"

namespace skidarake {

shared_ptr<Texture2D> TextureCache::FetchFromPath(const string& path) {
    unsigned int texture_id = HashedString(path).id();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Application::Instance().loader().ReadTexture(path);
        if (texture) {
            textures_[texture_id] = texture;
        } else {
            return texture;
        }
    }

    return textures_[texture_id];
}

}
