#ifndef __sometuke__ios_ios_file_utils__
#define __sometuke__ios_ios_file_utils__

#include "sometuke/application_component.h"
#include "sometuke/ios/devices.h"

namespace sometuke {

class IOSdImageLoader : public ImageLoader {
public:
    IOSImageLoder() {
        suffixes_[kiPhone]              = "";
        suffixes_[kiPhoneRetinaDisplay] = "@x2";
        suffixes_[kiPad]                = "-ipad";
        suffixes_[kiPadRetinaDisplay]   = "-ipad@x2";
    }

    const string FullPathFromRelativePath(const string& relative_path);

private:
    bool EndsWith(const string& str, const string& suffix) {
        if (str.length() >= suffix.length()) {
            return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
        } else {
            return false;
        }
    }

    string suffixes_[kDevice_MAX];
};
    
}

#endif /* defined(__sometuke___ios_ios_file_utils__) */
