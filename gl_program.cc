#include "gl_program.h"

#include <vector>
#include <iostream>

#include "file_utils.h"
#include "logger.h"
#include "matrix_stack.h"

namespace kawaii {

GLProgram::GLProgram() : vertShader_(0),
                         fragShader_(0) {
    program_ = glCreateProgram();
    for (int i = 0; i < kUniform_MAX; ++i) {
        uniform_locations_[i] = -1;
    }
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

    UpdateUniformLocations();
    
    return true;
}

void GLProgram::AddAttribute(const string& attributeName, const VertexAttribIndex index) {
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

void GLProgram::UpdateUniformLocations() {
    uniform_locations_[kUniform_PMatrix]   = glGetUniformLocation(program_, UNIFORM_NAME_PMatrix);
    uniform_locations_[kUniform_MVMatrix]  = glGetUniformLocation(program_, UNIFORM_NAME_MVMatrix);
    uniform_locations_[kUniform_MVPMatrix] = glGetUniformLocation(program_, UNIFORM_NAME_MVPMatrix);

    uniform_locations_[kUniform_Time]    = glGetUniformLocation(program_, UNIFORM_NAME_Time);
    uniform_locations_[kUniform_SinTime] = glGetUniformLocation(program_, UNIFORM_NAME_SinTime);
    uniform_locations_[kUniform_CosTime] = glGetUniformLocation(program_, UNIFORM_NAME_CosTime);
    usesTime_ = (uniform_locations_[kUniform_Time] ||
                 uniform_locations_[kUniform_SinTime] ||
                 uniform_locations_[kUniform_CosTime]);

    uniform_locations_[kUniform_Random01] = glGetUniformLocation(program_, UNIFORM_NAME_Random01);

    uniform_locations_[kUniform_Sampler]  = glGetUniformLocation(program_, UNIFORM_NAME_Sampler);
    
    Use();
    glUniform1i(uniform_locations_[kUniform_Sampler], 0);
}

void GLProgram::SetUniformsForBuiltins() {
    mat4 projection = MatrixStack::GLProjection()->Get();
    mat4 model_view = MatrixStack::GLModelView()->Get();
    mat4 model_view_projection = model_view * projection;

    // glUniformMatrix4fv(uniform_MVPMatrix_, 1, GL_FALSE, model_view_projection.Pointer());
    SetUniformMatrix4fv(kUniform_MVPMatrix, model_view_projection.Pointer());
}

void GLProgram::SetUniformMatrix4fv(const UniformLabel label, const GLfloat *value) {
    glUniformMatrix4fv(uniform_locations_[label], 1, GL_FALSE, value);
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
