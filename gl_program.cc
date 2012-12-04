#include "gl_program.h"

#include <vector>
#include <iostream>

#include "file_utils.h"
#include "logger.h"
#include "matrix_stack.h"

namespace kawaii {

GLProgram::GLProgram() : vertShader_(0),
                         fragShader_(0),
                         uniform_MVPMatrix_(0),
                         uniform_Sampler_(0) {
    program_ = glCreateProgram();
}

bool GLProgram::LoadShaderFiles(const string& vShaderFilename, const string& fShaderFilename) {
    string vShaderSource = ReadStringResource(vShaderFilename);
    string fShaderSource = ReadStringResource(fShaderFilename);
    
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

    // set default uniforms
    uniform_MVPMatrix_ = glGetUniformLocation(program_, kUniformName_MVPMatrix);
    uniform_Sampler_   = glGetUniformLocation(program_, kUniformName_Sampler);

    Use();
    glUniform1i(uniform_Sampler_, 0);
    
    return true;
}

void GLProgram::AddAttribute(const string& attributeName, GLuint index) {
    glBindAttribLocation(program_, index, attributeName.c_str());
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

void GLProgram::SetUniformsForBuiltins() {
    mat4 projection = MatrixStack::GLProjection()->Get();
    mat4 model_view = MatrixStack::GLModelView()->Get();
    mat4 model_view_projection = model_view * projection;

    // glUniformMatrix4fv(uniform_MVPMatrix_, 1, GL_FALSE, model_view_projection.Pointer());
    SetUniformMatrix4fv(uniform_MVPMatrix_, model_view_projection.Pointer());
}

void GLProgram::SetUniformMatrix4fv(const GLint uniform, const GLfloat *value) {
    glUniformMatrix4fv(uniform, 1, GL_FALSE, value);
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

// const string &Director::VertexShaderLog() const {
// }

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
