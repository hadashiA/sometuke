#ifndef __kawaii__shader_cache__
#define __kawaii__shader_cache__

#include <memory>

#include <OpenGLES/ES2/gl.h>

#include "gl_program.h"

namespace kawaii {
using namespace std;

typedef enum {
    kShader_PositionTextureColor,
    kShader_PositionTextureColorAlphaText,
    kShader_PositionColor,
    kShader_PositionTexture,
    kShader_PositionTexture_uColor,
    kShader_PositionTextureA8Color,
    kShader_Position_uColor,
    kShader_PositionLengthTextureColor,

    kShaderLabel_Max,
} ShaderLabel;

class ShaderCache {
public:
    static ShaderCache *Shared();

    shared_ptr<GLProgram> operator[](ShaderLabel key);
    void LoadDefaultShaders();

private:
    ShaderCache() {}

    static ShaderCache *__shared;

    shared_ptr<GLProgram> shaders_[kShaderLabel_Max];
};

}

#endif /* defined(__kawaii__shader_cache__) */
