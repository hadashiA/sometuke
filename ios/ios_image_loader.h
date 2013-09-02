#ifndef __sometuke__ios_ios_image_loader__
#define __sometuke__ios_ios_image_loader__

#include "sometuke/application_component.h"
#include "sometuke/ios/devices.h"

namespace sometuke {

using namespace std;

class IOSImageLoader : public ImageLoader {
public:
    shared_ptr<Image> CreateImageFromFile(const string& path, Image::Format format);
};

} 

#endif /* defined(__sometuke__ios_image_loader__) */
