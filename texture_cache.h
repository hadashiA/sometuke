#ifndef __kawaii__texture_cache__
#define __kawaii__texture_cache__

#include <memory>
#include <map>

namespace kawaii {
using namespace std;

class Texture2D;

class TextureCache {
public:
    static TextureCache *Current();

    shared_ptr<Texture2D> AddImage(const string& path);

private:
    typedef std::map<unsigned int, shared_ptr<Texture2D> > TextureTable;

    static TextureCache *__current;

    TextureCache() {}
    TextureTable textures_;
};

}

#endif /* defined(__kawaii__texture_cache__) */
