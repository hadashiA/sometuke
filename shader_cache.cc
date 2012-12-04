#include "shader_cache.h"

namespace kawaii {

ShaderCache *ShaderCache::__shared = NULL;
ShaderCache *ShaderCache::Shared() {
    if (__shared == NULL) {
        __shared = new ShaderCache();
    }
    return __shared;
}

void ShaderCache::Purge() {
    delete __shared;
    __shared = NULL;
}

GLProgram *ShaderCache::operator[](ShaderLabel key) {
    return shaders_[key];
}

void LoadDefaultShaders() {
}

}
