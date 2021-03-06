#ifndef __sometuke__ios_devices__
#define __sometuke__ios_devices__

#include "sometuke/logger.h"

#include <OpenGLES/ES2/gl.h>

// TODO: Device staff that separates file
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#define __S2_PLATFORM_IOS 1
#endif

namespace sometuke {

enum {
    kiOSVersion_4_0   = 0x04000000,
    kiOSVersion_4_0_1 = 0x04000100,
    kiOSVersion_4_1   = 0x04010000,
    kiOSVersion_4_2   = 0x04020000,
    kiOSVersion_4_2_1 = 0x04020100,
    kiOSVersion_4_3   = 0x04030000,
    kiOSVersion_4_3_1 = 0x04030100,
    kiOSVersion_4_3_2 = 0x04030200,
    kiOSVersion_4_3_3 = 0x04030300,
    kiOSVersion_4_3_4 = 0x04030400,
    kiOSVersion_4_3_5 = 0x04030500,
    kiOSVersion_5_0   = 0x05000000,
    kiOSVersion_5_0_1 = 0x05000100,
    kiOSVersion_5_1_0 = 0x05010000,
    kiOSVersion_6_0_0 = 0x06000000,
};

typedef enum {
    kiPhone,
    kiPhoneRetinaDisplay,
    kiPad,
    kiPadRetinaDisplay,

    kDevice_MAX,
} DeviceType;

static inline const DeviceType RunningDevice() {
    static DeviceType __running_device = kDevice_MAX;
    if (__running_device == kDevice_MAX) {
        float content_scale_factor = [[UIScreen mainScreen] scale];

        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            __running_device = (content_scale_factor == 2 ? kiPadRetinaDisplay : kiPad);
        } else {
            __running_device = (content_scale_factor == 2 ? kiPhoneRetinaDisplay : kiPhone);
        }
    }
    return __running_device;
}

static inline const unsigned int RunningVersion() {
    static unsigned int __ios_version = 0;

    if (__ios_version == 0) {
        NSString *versionStr = [[UIDevice currentDevice] systemVersion];
        NSArray *arr = [versionStr componentsSeparatedByString:@"."];
        int idx = 0x01000000;
        for( NSString *str in arr ) {
            int value = [str intValue];
            __ios_version += value * idx;
            idx = idx >> 8;
        }
        S2INFO("iOS version: %s (0x%08x)", [versionStr UTF8String], __ios_version);
    }

    return __ios_version;
}

}

#endif
