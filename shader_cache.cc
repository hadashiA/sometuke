#include "shader_cache.h"
#include "OpenGL_Internal.h"

namespace kawaii {

ShaderCache::~ShaderCache() {
    for (int i = 0; i < kShaderLabel_Max; ++i) {
        GLProgram *shader = shaders_[i];
        if (shader != NULL) {
            delete shader;
            shaders_[i] = NULL;
        }
    }
}

ShaderCache *ShaderCache::__shared = NULL;
ShaderCache *ShaderCache::Shared() {
    if (__shared == NULL) {
        __shared = new ShaderCache();
    }
    return __shared;
}

void ShaderCache::Purge() {
    if (__shared != NULL) {
        delete __shared;
        __shared = NULL;
    }
}

GLProgram *ShaderCache::operator[](ShaderLabel key) {
    return shaders_[key];
}

void ShaderCache::LoadDefaultShaders() {
    GLProgram *p = new GLProgram();
    p->LoadShaderFiles("position_ucolor.vert", "position_ucolor.frag");
    p->AddAttribute("a_Position", kVertexAttrib_Position);
    p->Link();
    CHECK_GL_ERROR_DEBUG();
}

}
