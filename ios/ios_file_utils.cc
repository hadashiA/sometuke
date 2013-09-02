#include "sometuke/ios/ios_file_utils.h"

#include "sometuke/application_component.h"
#include "sometuke/logger.h"

#include <UIKit/UIKit.h>
#include <fstream>

namespace sometuke {

const string IOSFileUtils::FullPathFromRelativePath(const string& relative_path) {
    DeviceType d = RunningDevice();

    string suffix = suffixes_[d];
        
    int ext_pos = relative_path.find_last_of('.');
    string extname  = (ext_pos == string::npos ? "" : relative_path.substr(ext_pos));

    string relative_path_with_suffix = relative_path;
    
    if (!suffix.empty() && extname != ".frag" && extname != ".vert") {
        string basename = relative_path.substr(0, ext_pos);

        if (basename.find(suffix) == string::npos) {
            if (extname == ".ccz" || extname == ".gz") {
                ext_pos = basename.find_last_of('.');
                if (ext_pos != string::npos) {
                    extname.insert(0, basename.substr(ext_pos));
                    basename.erase(ext_pos);
                }
            }
            relative_path_with_suffix = basename + suffix + extname;

        } else {
            IIWARN("filename:%s already has the suffix %s. Using it.",
                    relative_path.c_str(), suffix.c_str());
        }
    }
    int dir_pos = relative_path_with_suffix.find_last_of('/');
    string dirname, filename;
    if (dir_pos == string::npos) {
        dirname  = "";
        filename = relative_path_with_suffix;
    } else {
        dirname  = relative_path_with_suffix.substr(0, dir_pos);
        filename = relative_path_with_suffix.substr(dir_pos);
    }

    NSString *result_ns =
        [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filename.c_str()]
                                        ofType:nil
                                   inDirectory:[NSString stringWithUTF8String:dirname.c_str()]];

    if (!result_ns) {
        IIWARN("ios_assets_loader: file not found: %s", relative_path_with_suffix.c_str());
        return string("");
    } else {
        return string([result_ns UTF8String]);
    }
}

}

