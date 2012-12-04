#ifndef __kawaii__shader_cache__
#define __kawaii__shader_cache__

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
    ~ShaderCache();

    static ShaderCache *Shared();
    static void Purge();

    GLProgram *operator[](ShaderLabel key);
    void LoadDefaultShaders();

private:
    ShaderCache() {}

    static ShaderCache *__shared;

    GLProgram *shaders_[kShaderLabel_Max];
};

}

#endif /* defined(__kawaii__shader_cache__) */
