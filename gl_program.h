#ifndef __kawaii__gl_program__
#define __kawaii__gl_program__

#include <OpenGLES/ES2/gl.h>
#include <string>

#include "matrix.h"

namespace kawaii {
using namespace std;

typedef enum {
    kVertexAttrib_Position,
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

static const char *UNIFORM_NAME_PMatrix        = "u_PMatrix";
static const char *UNIFORM_NAME_MVMatrix       = "u_MVMatrix";
static const char *UNIFORM_NAME_MVPMatrix      = "u_MVPMatrix";
static const char *UNIFORM_NAME_Time           = "u_Time";
static const char *UNIFORM_NAME_SinTime        = "u_SinTime";
static const char *UNIFORM_NAME_CosTime        = "u_CosTime";
static const char *UNIFORM_NAME_Random01       = "u_Random01";
static const char *UNIFORM_NAME_Sampler        = "u_Sampler";

static const char *ATTRIBUTE_NAME_Position = "a_Position";
static const char *ATTRIBUTE_NAME_Color    = "a_Color";
static const char *ATTRIBUTE_NAME_TexCoord = "a_TexCoord";

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

    void AddAttribute(const string& attributeName, const VertexAttribIndex index);
    bool Link();
    void Use();

    void UpdateUniformLocations();
    void SetUniformsForBuiltins();
    void SetUniformMatrix4fv(const UniformLabel label, const GLfloat *value);

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
