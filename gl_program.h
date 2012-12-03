#ifndef __kawaii__gl_program__
#define __kawaii__gl_program__

#include <OpenGLES/ES2/gl.h>
#include <string>

#include "matrix.h"

namespace kawaii {
using namespace std;

enum {
    kVertexAttrib_Position,
    kVertexAttrib_Color,
    kVertexAttrib_TexCoords,

    kVertexAttrib_MAX,
};

#define kUniformName_MVPMatrix      "u_MVPMatrix"
#define kUniformName_Sampler        "u_Texture"
#define kUniformName_AlphaTestValue "u_AlphaValue"

#define kAttributeName_Position     "a_Position"
#define kAttributeName_Color        "a_Color"
#define kAttributeName_TexCoord     "a_TexCoord"


typedef void (*GLInfoFunction)(GLuint program,
                               GLenum pname,
                               GLint *params);
typedef void (*GLLogFunction) (GLuint program,
                               GLsizei bufsize,
                               GLsizei *length,
                               GLchar *infolog);

class GLProgram {
public:
    GLProgram();

    bool LoadShaderFiles(const string& vShaderFilename, const string& fShaderFilename);
    bool LoadShaderSources(const GLchar *vShaderSource, const GLchar *fShaderSource);

    const GLuint id() const {
        return program_;
    };

    const string VertexShaderLog() const;
    const string FragmentShaderLog() const;
    const string ProgramLog() const;

    void AddAttribute(const string& attributeName, GLuint index);
    bool Link();
    void Use();

    void SetUniformForModelViewProjectionMatrix(const mat4& model_view_projection);

private:
    bool CompileShader(GLuint *shader, GLenum type, const GLchar *source);

    const string LogForOpenGLObject(GLuint id,
                                    GLInfoFunction infoFunc,
                                    GLLogFunction logFunc) const;

    GLuint program_;
    GLuint vertShader_;
    GLuint fragShader_;

    GLint uniform_MVPMatrix_;
    GLint uniform_Sampler_;
};

} // namespace kawaii

#endif /* defined(__kawaii__gl_program__) */
