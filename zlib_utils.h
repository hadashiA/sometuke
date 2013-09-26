#ifndef __sometuke__zlib_utils__
#define __sometuke__zlib_utils__

#include <vector>

namespace sometuke {
using namespace std;

vector<unsigned char> inflate_memory_with_hint(vector<unsigned char>& in,
                                               size_t out_size_hint);
}

#endif
