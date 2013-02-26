#ifndef __kawaii__texture_cache__
#define __kawaii__texture_cache__

#include <memory>
#include <unordered_map>

namespace kawaii {
using namespace std;

class Texture2D;

class TextureCache {
public:
    static unique_ptr<TextureCache>& Shared() {
        static unique_ptr<TextureCache> __shared(new TextureCache);
        return __shared;
    }

    shared_ptr<Texture2D> FetchFromPath(const string& path);

private:
    typedef unordered_map<unsigned int, shared_ptr<Texture2D> > TextureTable;

    TextureCache() {}
    TextureTable textures_;
};

}

#endif /* defined(__kawaii__texture_cache__) */
