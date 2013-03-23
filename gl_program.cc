#include "gl_program.h"

#include "application.h"
#include "assets.h"
#include "logger.h"
#include "matrix_stack.h"

#include <vector>
#include <iostream>

namespace kawaii {

const GLchar *UNIFORM_NAMES[kUniform_MAX] = {
    "u_PMatrix",
    "u_MVMatrix",
    "u_MVPMatrix",
    "u_Time",
    "u_SinTime",
    "u_CosTime",
    "u_Random01",
    "u_Sampler",
};

const GLchar *VERTEX_ATTRIB_NAMES[kVertexAttrib_MAX] = {
    "a_Position",
    "a_Color",
    "a_TexCoord"
};


GLProgram::GLProgram()
    : vertShader_(0),
      fragShader_(0) {
    program_ = glCreateProgram();
    for (int i = 0; i < kUniform_MAX; ++i) {
        uniform_locations_[i] = -1;
    }
}

bool GLProgram::LoadShaderFiles(const string& vShaderFilename, const string& fShaderFilename) {
    Application& application = Application::Instance();
    string vShaderSource = application.assets().ReadString(vShaderFilename);
    string fShaderSource = application.assets().ReadString(fShaderFilename);
    
    return LoadShaderSources(vShaderSource.c_str(), fShaderSource.c_str());
}

bool GLProgram::LoadShaderSources(const GLchar *vShaderSource, const GLchar *fShaderSource) {
    if (vertShader_) {
        glDeleteShader(vertShader_);
    }
    if (fragShader_) {
        glDeleteShader(fragShader_);
    }

    vertShader_ = 0;
    fragShader_ = 0;

    if (!CompileShader(&vertShader_, GL_VERTEX_SHADER, vShaderSource)) {
        IIERROR("Failed to compile vertex shader");
        return false;
    }
    if (!CompileShader(&fragShader_, GL_FRAGMENT_SHADER, fShaderSource)) {
        IIERROR("Failed to compile fragment shader");
    }

    if (vertShader_) {
        glAttachShader(program_, vertShader_);
    }
    if (fragShader_) {
        glAttachShader(program_, fragShader_);
    }

    if (!Link()) {
        return false;
    }

    UpdateUniformLocations();
    
    return true;
}

void GLProgram::AddAttribute(const VertexAttribIndex index) {
    const GLchar *attribName = VERTEX_ATTRIB_NAMES[index];
    glBindAttribLocation(program_, index, attribName);
}

void GLProgram::Use() {
    glUseProgram(program_);
}

bool GLProgram::Link() {
    glLinkProgram(program_);

#if DEBUG
    GLint status;
    glValidateProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        IIERROR("Failed to link program: %i", program_);
        if (vertShader_) {
            glDeleteShader(vertShader_);
        }
        if (fragShader_) {
            glDeleteShader(fragShader_);
        }
        glDeleteProgram(program_);
        vertShader_ = fragShader_ = program_ = 0;
        return false;
    }
#endif

    if (vertShader_) {
        glDeleteShader(vertShader_);
    }
    if (fragShader_) {
        glDeleteShader(fragShader_);
    }
    vertShader_ = fragShader_ = 0;

    return true;
}

void GLProgram::UpdateUniformLocations() {
    for (int i = 0; i < kUniform_MAX; ++i) {
        uniform_locations_[i] = glGetUniformLocation(program_, UNIFORM_NAMES[i]);
    }

    usesTime_ = (uniform_locations_[kUniform_Time] != -1 ||
                 uniform_locations_[kUniform_SinTime] != -1  ||
                 uniform_locations_[kUniform_CosTime] != -1);

    Use();
    SetUniform1i(kUniform_Sampler, 0);
}

void GLProgram::SetUniformsForBuiltins() {
    mat4& projection = MatrixStack<GLProjection>::Instance().Top();
    mat4& model_view = MatrixStack<GLModelView>::Instance().Top();
    mat4 model_view_projection = model_view * projection;

    SetUniformMatrix4fv(kUniform_MVPMatrix, model_view_projection.Pointer());
}

void GLProgram::SetUniformMatrix4fv(const UniformLabel label, const GLfloat *value) {
    glUniformMatrix4fv(uniform_locations_[label], 1, GL_FALSE, value);
}

void GLProgram::SetUniform1i(const UniformLabel label, GLint v0) {
    glUniform1i(uniform_locations_[label], v0);
}

void GLProgram::SetUniform1f(const UniformLabel label, GLfloat v0) {
    glUniform1i(uniform_locations_[label], v0);
}

// private

bool GLProgram::CompileShader(GLuint *shader, GLenum type, const GLchar *source) {
    GLint status;
    if (!source) return false;

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
    glGetShaderiv(static_cast<GLuint>(*shader), GL_COMPILE_STATUS, &status);
    if (!status) {
        cout << this->LogForOpenGLObject(*shader, &glGetShaderiv, &glGetShaderInfoLog) << endl;
    }
    return (status == GL_TRUE);
}

const string GLProgram::VertexShaderLog() const {
    return this->LogForOpenGLObject(vertShader_, &glGetShaderiv, &glGetShaderInfoLog);
}

const string GLProgram::FragmentShaderLog() const {
    return this->LogForOpenGLObject(fragShader_, &glGetShaderiv, &glGetShaderInfoLog);
}

const string GLProgram::ProgramLog() const {
    return this->LogForOpenGLObject(program_, &glGetProgramiv, &glGetProgramInfoLog);
}

const string GLProgram::LogForOpenGLObject(GLuint id,
                                           GLInfoFunction infoFunc,
                                           GLLogFunction logFunc) const {
    GLint logLength    = 0;
    GLint charsWritten = 0;

    infoFunc(id, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1) {
        return string("");
    }

    vector<char> log(logLength);
    logFunc(id, logLength, &charsWritten, &log[0]);
    return string(log.begin(), log.end());
}

}
