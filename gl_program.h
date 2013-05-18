#ifndef __kawaii__gl_program__
#define __kawaii__gl_program__

#include <OpenGLES/ES2/gl.h>
#include <string>

#include "kawaii/matrix.h"

namespace kawaii {
using namespace std;

typedef enum {
    kVertexAttrib_Position = 0,
    kVertexAttrib_Color,
    kVertexAttrib_TexCoords,

    kVertexAttrib_MAX,
} VertexAttribIndex;

typedef enum {
    kUniform_PMatrix,
    kUniform_MVMatrix,
    kUniform_MVPMatrix,
    kUniform_Time,
    kUniform_SinTime,
    kUniform_CosTime,
    kUniform_Random01,
    kUniform_Sampler,

    kUniform_MAX,
} UniformLabel;

extern const GLchar *VERTEX_ATTRIB_NAMES[kVertexAttrib_MAX];
extern const GLchar *UNIFORM_NAMES[kUniform_MAX];

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

    void AddAttribute(const VertexAttribIndex index);
    bool Link();
    void Use();

    void UpdateUniformLocations();
    void SetUniformsForBuiltins();
    void SetUniformMatrix4fv(const UniformLabel label, const GLfloat *value);
    void SetUniform1i(const UniformLabel label, GLint v0);
    void SetUniform1f(const UniformLabel label, GLfloat v0);

private:
    bool CompileShader(GLuint *shader, GLenum type, const GLchar *source);

    const string LogForOpenGLObject(GLuint id,
                                    GLInfoFunction infoFunc,
                                    GLLogFunction logFunc) const;
    GLuint program_;
    GLuint vertShader_;
    GLuint fragShader_;

    GLint uniform_locations_[kUniform_MAX];
    bool usesTime_;
};


} // namespace kawaii

#endif /* defined(__kawaii__gl_program__) */
