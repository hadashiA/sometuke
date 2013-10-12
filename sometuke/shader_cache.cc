#include "sometuke/shader_cache.h"
#include "sometuke/OpenGL_Internal.h"

namespace sometuke {

void ShaderCache::LoadDefaultShaders() {
    {
        auto p = make_shared<GLProgram>();
        p->LoadShaderFiles("position_ucolor.vert", "position_ucolor.frag");
        p->AddAttribute(kVertexAttrib_Position);
        CHECK_GL_ERROR_DEBUG();
        shaders_[kShader_Position_uColor] = p;
    }
    
    {
        auto p = make_shared<GLProgram>();
        p->LoadShaderFiles("position_color.vert", "position_color.frag");
        p->AddAttribute(kVertexAttrib_Position);
        p->AddAttribute(kVertexAttrib_Color);
        CHECK_GL_ERROR_DEBUG();
        shaders_[kShader_PositionColor] = p;
    }

    {
        auto p = make_shared<GLProgram>();
        p->LoadShaderFiles("position_texture.vert", "position_texture.frag");
        p->AddAttribute(kVertexAttrib_Position);
        p->AddAttribute(kVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        shaders_[kShader_PositionTexture] = p;
    }
    
    {
        auto p = make_shared<GLProgram>();
        p->LoadShaderFiles("position_texture_color.vert", "position_texture_color.frag");
        p->AddAttribute(kVertexAttrib_Position);
        p->AddAttribute(kVertexAttrib_Color);
        p->AddAttribute(kVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        shaders_[kShader_PositionTextureColor] = p;
    }

    {
        auto p = make_shared<GLProgram>();
        p->LoadShaderFiles("position_texture_ucolor.vert", "position_texture_ucolor.frag");
        p->AddAttribute(kVertexAttrib_Position);
        p->AddAttribute(kVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        shaders_[kShader_PositionTexture_uColor] = p;
    }
}

}
