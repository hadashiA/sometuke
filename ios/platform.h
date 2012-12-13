#ifndef __kawaii__ios__platform__
#define __kawaii__ios__platform__

// TODO: Device staff that separates file
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#define __II_PLATFORM_IOS 1
#endif

namespace kawaii {

typedef enum {
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
} kiOSVersion;

typedef enum {
    kiPhone,
    kiPhoneRetinaDisplay,
    kiPad,
    kiPadRetinaDisplay,

    kDevice_MAX,
} DeviceType;


static const DeviceType RunningDevice() {
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

}

#endif
