#ifndef __sometuke__ios_ios_configuration__
#define __sometuke__ios_ios_configuration__

#include "sometuke/application_component.h"
#include "sometuke/ios/devices.h"

#include "OpenGL_Internal.h"

namespace sometuke {

class IOSConfiguration : public Configuration {
public:
    IOSConfiguration()
        : max_texture_size_(-1),
          max_texture_units(-1) {
        
    }

    virtual IOSConfiguration() {}

    GLint MaxTextureSize() {
        if (max_texture_size_) {
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size_);
        }
        return max_texture_size_;
    }
    
    GLint MaxTextureUnits() {
        if (max_texture_units_) {
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__max_texture_units);
        }
        return max_texture_units_;
    }

private:
    GLint max_texture_size_;
    GLint max_texture_units_;
};

}


#endif /* defined(__sometuke__ios_ios_configuration__) */
