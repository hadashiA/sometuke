#ifndef __kawaii__shader_cache__
#define __kawaii__shader_cache__

#include <memory>

#include <OpenGLES/ES2/gl.h>

#include "kawaii/gl_program.h"

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
    static ShaderCache& Instance() {
        static unique_ptr<ShaderCache> __instance;
        if (!__instance) {
            __instance.reset(new ShaderCache);
            __instance->LoadDefaultShaders();
        }

        return *__instance;
    }

    shared_ptr<GLProgram> operator[](ShaderLabel key) {
        shared_ptr<GLProgram> result = shaders_[key];
        return result;
    }

    shared_ptr<GLProgram> get(ShaderLabel key) {
        shared_ptr<GLProgram> result = shaders_[key];
        return result;
    }

    void LoadDefaultShaders();

private:
    ShaderCache() {}

    ShaderCache(const ShaderCache&);
    ShaderCache& operator=(const ShaderCache&);

    shared_ptr<GLProgram> shaders_[kShaderLabel_Max];
};

}

#endif /* defined(__kawaii__shader_cache__) */
