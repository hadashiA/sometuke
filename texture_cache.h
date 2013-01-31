#ifndef __kawaii__texture_cache__
#define __kawaii__texture_cache__

#include <memory>
#include <map>

namespace kawaii {
using namespace std;

class Texture2D;

class TextureCache {
public:
    static inline TextureCache *Shared() {
        if (__shared == NULL) {
            __shared = new TextureCache;
        }
        return __shared;
    }

    shared_ptr<Texture2D> FetchFromPath(const string& path);

private:
    typedef std::map<unsigned int, shared_ptr<Texture2D> > TextureTable;

    static TextureCache *__shared;

    TextureCache() {}
    TextureTable textures_;
};

}

#endif /* defined(__kawaii__texture_cache__) */
