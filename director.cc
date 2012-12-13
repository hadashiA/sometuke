#include "director.h"
#include "shader_cache.h"
#include "logger.h"

#include <OpenGLES/ES2/gl.h>

#include "OpenGL_Internal.h"
#include "matrix_stack.h"

namespace kawaii {

Director *Director::__current = NULL;
Director *Director::Current() {
    if (__current == NULL) {
        __current = new Director();
    }
    return __current;
}

void Director::ReshapeProjection(const float width, const float height) {
    const vec2 size_in_points(width, height);
    const vec2 size_in_pixels = size_in_points * content_scale_factor_;
    
    IIINFO("SetProjection %fx%f", size_in_points.x, size_in_points.y);
    glViewport(0, 0, size_in_points.x, size_in_points.y);

    float zeye = size_in_pixels.y / 1.1566f / content_scale_factor_;
    mat4 projection = mat4::Perspective(60, size_in_points.x / size_in_points.y,
                                        0.1f, zeye * 2);
    MatrixStack::GLProjection()->Push(projection);

    vec3 eye(size_in_points.x / 2, size_in_points.y / 2, zeye);
    vec3 center(size_in_points.x / 2, size_in_points.y / 2, 0);
    vec3 up(0, 1, 0);
    mat4 model_view = mat4::LookAt(eye, center, up);
    MatrixStack::GLModelView()->Push(model_view);

    CHECK_GL_ERROR();
}

void Director::MainLoop(float delta_time) {
    total_time_ += delta_time;

    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderCache shaders = *ShaderCache::Shared();
    shared_ptr<GLProgram> p = shaders[kShader_Position_uColor];
    p->SetUniformsForBuiltins();

    GLint color_location = glGetUniformLocation(p->id(), "u_Color");
    vec4 color(1, 0, 0, 1);
    glUniform4fv(color_location, 1, color.Pointer());

    GLint point_size_location = glGetUniformLocation(p->id(), "u_PointSize");
    glUniform1f(point_size_location, 20);

    // attributes
    glEnableVertexAttribArray(kVertexAttrib_Position);

    vec2 pos(10 + total_time_, 10);
    glVertexAttribPointer(kVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pos.Pointer());

    glDrawArrays(GL_POINTS, 0, 1);
}

} // namespace kawaii




