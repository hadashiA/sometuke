#ifndef __sometuke__tmx_parser__
#define __sometuke__tmx_parser__

namespace sometuke {

enum class TmxFormat {
    CSV,
    XML,
    BASE64,
    GZIP,
    ZLIB,
};

enum class TmxNode {
    MAP,
    LAYER,
    OBJECT_GROUP,
    OBJECT,
    TILE,
};


}

#endif /* defined(__sometuke__tmx_parser__) */
