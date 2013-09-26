#include "sometuke/zlib_utils.h"

#include "sometuke/logger.h"

#include <stdlib.h>
#include <zlib.h>

#define BUFFER_INC_FACTOR (2)

namespace sometuke {

vector<unsigned char> inflate_memory_with_hint(const vector<unsigned char>& in,
                                               size_t out_size_hint) {
    int err = Z_OK;
    size_t buffer_size = out_size_hint;
    vector<unsigned char> result;
    result.reserve(buffer_size);

    unsigned char *out = static_cast<unsigned char *>(malloc(buffer_size));

    z_stream stream;
    stream.zalloc    = Z_NULL;
    stream.zfree     = Z_NULL;
    stream.opaque    = Z_NULL;
    stream.next_in   = (unsigned char *)in.data();
    stream.next_out  = out;
    stream.avail_out = buffer_size;

    err = inflateInit2(&stream, 15 + 32);
    if (err != Z_OK) {
        return result;
    }
    
    for (;;) {
        err = inflate(&stream, Z_NO_FLUSH);

        if (err == Z_STREAM_END)
            break;
        switch (err) {
        case Z_NEED_DICT:
            err = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&stream);
            break;
        }

        // not enough memory ?
        if (err != Z_STREAM_END) {
            unsigned char *tmp = static_cast<unsigned char *>(realloc(out, buffer_size * BUFFER_INC_FACTOR));

            /* not enough memory, ouch */
            if (!tmp) {
                S2ERROR("zlib_utils: realloc failed");
                inflateEnd(&stream);
                return result;
            }
            /* only assign to *out if tmp is valid. it's not guaranteed that realloc will reuse the memory */
            out = tmp;
            
            stream.next_out = out + buffer_size;
            stream.avail_out = buffer_size;
            buffer_size *= BUFFER_INC_FACTOR;
        }
    }

    err = inflateEnd(&stream);

    if (err != Z_OK || out == nullptr) {
        if (err == Z_MEM_ERROR) {
            S2ERROR("zlib utils: out of memory while decompressiong.");
        } else if (err == Z_VERSION_ERROR) {
            S2ERROR("zlib utils: incompatible zlib version.");
        } else if (err == Z_DATA_ERROR) {
            S2ERROR("zlib utils: incorrect zlib compressed data.");
        } else {
            S2ERROR("zlib utils: unknown error while decompressiong.");
        }
        free(out);
        out = nullptr;
    }
    return result;
}

}
