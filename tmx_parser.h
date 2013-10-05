#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

#include "sometuke/tmx_types.h"
#include "sometuke/vector.h"
#include "sometuke/color.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace sometuke {
using namespace std;

class TmxParser {
public:
    static TmxParser& Instance() {
        static unique_ptr<TmxParser> __instance(new TmxParser);
        return *__instance;
    }

    shared_ptr<TmxTileMapInfo> Parse(const string& file);

private:
    TmxParser() {}

    TmxParser(const TmxParser&) = delete;
    TmxParser(TmxParser&&) = delete;
    TmxParser& operator=(const TmxParser&) = delete;
    TmxParser& operator=(TmxParser&&) = delete;

    bool ParseLayerData(shared_ptr<TmxLayerInfo>& layer_info,
                        const string& data,
                        const TmxEncoding encoding,
                        const TmxCompression compression);
};

}

#endif /* defined(__sometuke__tmx_parser__) */
