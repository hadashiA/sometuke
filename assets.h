#ifndef __kawaii__assets__
#define __kawaii__assets__

#include <vector>
#include <memory>

class Texture2D;

class Assets {
public:
    virtual vector<char> ReadResource(const string &relative_path) = 0;
    virtual shared_ptr<Texture2D> ReadTexture(const string &relative_path) = 0;
    virtual string ReadString(const string &relative_path) = 0;
};

#endif /* defined(__kawaii__resource_loader__) */
