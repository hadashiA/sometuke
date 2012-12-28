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
    shared_ptr<GLProgram> result = shaders_[key];
    return result;
}

void ShaderCache::LoadDefaultShaders() {
    shared_ptr<GLProgram> p;

    p.reset(new GLProgram);
    p->LoadShaderFiles("position_ucolor.vert", "position_ucolor.frag");
    p->AddAttribute(kVertexAttrib_Position);
    CHECK_GL_ERROR_DEBUG();
    shaders_[kShader_Position_uColor] = p;

    p.reset(new GLProgram);
    p->LoadShaderFiles("position_color.vert", "position_color.frag");
    p->AddAttribute(kVertexAttrib_Position);
    p->AddAttribute(kVertexAttrib_Color);
    CHECK_GL_ERROR_DEBUG();
    shaders_[kShader_PositionColor] = p;

    p.reset(new GLProgram);
    p->LoadShaderFiles("position_texture.vert", "position_texture.frag");
    p->AddAttribute(kVertexAttrib_Position);
    p->AddAttribute(kVertexAttrib_TexCoords);
    CHECK_GL_ERROR_DEBUG();
    shaders_[kShader_PositionTexture] = p;
}

}
