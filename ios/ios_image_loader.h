#ifndef __sometuke__ios_ios_image_loader__
#define __sometuke__ios_ios_image_loader__

#include "sometuke/application_component.h"
#include "sometuke/ios/devices.h"

namespace sometuke {

using namespace std;

class IOSImage : public Image {
public:    
};

class IOSImageLoader : public ImageLoader {
public:
    shared_ptr<Texture2D> CreateTextureFromFile(const string& path);
    shared_ptr<Image> Create() {
        return make_shared<IOSImage>();
    }
private:    
};

} 

#endif /* defined(__sometuke__ios_image_loader__) */
