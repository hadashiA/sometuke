#ifndef __sometuke__zlib_utils__
#define __sometuke__zlib_utils__

#include <vector>

namespace sometuke {
using namespace std;

unsigned char *inflate_memory_with_hint(unsigned char *in,
                                        size_t in_size,
                                        size_t *out_size,
                                        size_t out_size_hint);
}

#endif
