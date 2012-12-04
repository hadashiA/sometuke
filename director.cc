#include "director.h"
#include "file_utils.h"
#include "gl_program.h"
#include "logger.h"

#include <OpenGLES/ES2/gl.h>

#include "OpenGL_Internal.h"
#include "matrix_stack.h"

namespace kawaii {

void Director::SetProjection(float width, float height) {
    winSizeInPoints_ = vec2(width, height);
    winSizeInPixels_ = winSizeInPoints_ * content_scale_factor_;

    IIINFO("Director::SetProjection %fx%f", winSizeInPoints_.x, winSizeInPoints_.y);
    glViewport(0, 0, winSizeInPoints_.x, winSizeInPoints_.y);

    float zeye = winSizeInPixels_.y / 1.1566f / content_scale_factor_;
    mat4 projection = mat4::Perspective(60, winSizeInPoints_.x / winSizeInPoints_.y,
                                        0.1f, zeye * 2);
    MatrixStack::GLProjection()->Push(projection);

    vec3 eye(winSizeInPoints_.x / 2, winSizeInPoints_.y / 2, zeye);
    vec3 center(winSizeInPoints_.x / 2, winSizeInPoints_.y / 2, 0);
    vec3 up(0, 1, 0);
    mat4 model_view = mat4::LookAt(eye, center, up);
    MatrixStack::GLModelView()->Push(model_view);

    CHECK_GL_ERROR();
}

void Director::DrawScene() {
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLProgram p;
    p.LoadShaderFiles("position_ucolor.vert", "position_ucolor.frag");
    p.Use();

    // uniforms
    p.SetUniformsForBuiltins();

    GLint color_location = glGetUniformLocation(p.id(), "u_Color");
    vec4 color(1, 0, 0, 1);
    glUniform4fv(color_location, 1, color.Pointer());

    GLint point_size_location = glGetUniformLocation(p.id(), "u_PointSize");
    glUniform1f(point_size_location, 20);

    // attributes
    p.AddAttribute("a_Position", kVertexAttrib_Position);
    glEnableVertexAttribArray(kVertexAttrib_Position);

    vec2 pos(10, 10);
    glVertexAttribPointer(kVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pos.Pointer());

    glDrawArrays(GL_POINTS, 0, 1);
}

} // namespace kawaii
