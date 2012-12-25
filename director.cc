#include "director.h"
#include "shader_cache.h"
#include "logger.h"
#include "texture_2d.h"
#include "matrix_stack.h"
#include "vertices.h"

#include "OpenGL_Internal.h"

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

    texture_ = assets_->ReadTexture("kid.png");
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture_->id());

    Quad_P3F_C4B_T2F quad;
    Color4B tmp_color(255, 255, 255, 255);
    quad.bottom_left.color  = tmp_color;
    quad.bottom_right.color = tmp_color;
    quad.top_left.color     = tmp_color;
    quad.top_right.color    = tmp_color;

    if (texture_->has_premultipled_alpha()) {
        blend_func_src_ = GL_ONE;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
    } else {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
    }

    vec2 content_size = texture_->content_size();
    float content_width  = content_size.x;
    float content_height = content_size.y;

    vec2 pixel_size = texture_->pixel_size();
    float pixel_width  = pixel_size.x;
    float pixel_height = pixel_size.y;

    Rect rect(0, 0, content_width, content_height);

    float left, right, top, bottom;
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

    vec2 pos(10 + total_time_, 10 + total_time_);
    glVertexAttribPointer(kVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pos.Pointer());

    glDrawArrays(GL_POINTS, 0, 1);

    
}

} // namespace kawaii




