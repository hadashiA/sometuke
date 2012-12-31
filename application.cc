#include "application.h"
#include "shader_cache.h"
#include "logger.h"
#include "texture_2d.h"
#include "matrix_stack.h"

#include "OpenGL_Internal.h"

namespace kawaii {

Application *Application::__current = NULL;
Application *Application::Current() {
    if (__current == NULL) {
        __current = new Application();
    }
    return __current;
}

void Application::ReshapeProjection(const float width, const float height) {
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

    Color4B tmp_color(255, 255, 255, 255);
    quad_.bottom_left.color  = tmp_color;
    quad_.bottom_right.color = tmp_color;
    quad_.top_left.color     = tmp_color;
    quad_.top_right.color    = tmp_color;

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
    float x1 = rect.pos.x;
    float y1 = rect.pos.y;
    float x2 = x1 + rect.size.x;
    float y2 = y1 + rect.size.y;
    quad_.bottom_left.pos  = vec3(x1, y1, 0);
    quad_.bottom_right.pos = vec3(x2, y1, 0);
    quad_.top_left.pos     = vec3(x1, y2, 0);
    quad_.top_right.pos    = vec3(x2, y2, 0);

    float left   = rect.pos.x / pixel_width;
    float right  = (rect.pos.x + rect.size.x) / pixel_width;
    float top    = rect.pos.y / pixel_height;
    float bottom = (rect.pos.y + rect.size.y) / pixel_height;

    quad_.bottom_left.tex_coord.u  = left;
    quad_.bottom_left.tex_coord.v  = bottom;
    quad_.bottom_right.tex_coord.u = right;
    quad_.bottom_right.tex_coord.v = bottom;
    quad_.top_left.tex_coord.u     = left;
    quad_.top_left.tex_coord.v     = top;
    quad_.top_right.tex_coord.u    = right;
    quad_.top_right.tex_coord.v    = top;

    IIINFO("rect:%s pixel_width:%f, pixel_height:%f left:%f right:%f top:%f bottom:%f", IIINSPECT(rect), pixel_width, pixel_height, left, right, top, bottom);
    IIINFO("quad_.bottom_left.tex_coord:%s", IIINSPECT(quad_.bottom_left.tex_coord));
}

void Application::MainLoop(float delta_time) {
    total_time_ += delta_time;

    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderCache shaders = *ShaderCache::Shared();
    shared_ptr<GLProgram> p =
    shaders[kShader_PositionTextureColor];
    // texture_->shader_program();

    // shared_ptr<GLProgram> p = texture_->shader_program();
    p->Use();
    p->SetUniformsForBuiltins();

    // set blending
    if (blend_func_src_ == GL_ONE && blend_func_dst_ == GL_ZERO) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(blend_func_src_, blend_func_dst_);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id());

    // attributes
    glEnableVertexAttribArray(kVertexAttrib_Position);
    glEnableVertexAttribArray(kVertexAttrib_Color);
    glEnableVertexAttribArray(kVertexAttrib_TexCoords);

#define VERTEX_SIZE sizeof(quad_.bottom_left)
    long offset = (long)&quad_;
    unsigned int diff;

    // position
    diff = offsetof(P3F_C4B_T2F, pos);
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (void *)(offset + diff));

    // texCoord
    diff = offsetof(P3F_C4B_T2F, tex_coord);
    glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE,
                          (void *)(offset + diff));

    // color
    diff = offsetof(P3F_C4B_T2F, color);
    glVertexAttribPointer(kVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE,
                          (void *)(offset + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_GL_ERROR_DEBUG();
}

} // namespace kawaii


