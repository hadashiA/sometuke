#include "shader_cache.h"
#include "OpenGL_Internal.h"

namespace kawaii {

ShaderCache *ShaderCache::__shared = NULL;
ShaderCache *ShaderCache::Shared() {
    if (__shared == NULL) {
        __shared = new ShaderCache();
        __shared->LoadDefaultShaders();
    }
    return __shared;
}

shared_ptr<GLProgram> ShaderCache::operator[](ShaderLabel key) {
    return shaders_[key];
}

void ShaderCache::LoadDefaultShaders() {
    shared_ptr<GLProgram> p(new GLProgram);
    p->LoadShaderFiles("position_ucolor.vert", "position_ucolor.frag");
    p->AddAttribute(ATTRIBUTE_NAME_Position, kVertexAttrib_Position);
    CHECK_GL_ERROR_DEBUG();
    shaders_[kShader_Position_uColor] = p;
}

}
