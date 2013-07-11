#include "skidarake/drawing_primitives.h"

#include "skidarake/shader_cache.h"
#include "skidarake/color.h"
#include "skidarake/OpenGL_Internal.h"

#include <memory>

namespace skidarake {

static bool __initialized = false;
static shared_ptr<GLProgram> gl_program_;
static GLint color_location_ = -1;
static Color4F color_(1,1,1,1);
static int point_size_location_ = -1;
static GLfloat point_size_ = 1;

static void __lazy_init() {
    if (!__initialized) {
        gl_program_ = ShaderCache::Instance()[kShader_Position_uColor];
        color_location_      = glGetUniformLocation(gl_program_->id(), "u_Color");
        point_size_location_ = glGetUniformLocation(gl_program_->id(), "u_PointSize");
        __initialized = true;
    }    
}

void DrawColor(const Color4B& color) {
    color_ = Color4F(color);
}

void DrawColor(const Color4F& color) {
    color_ = color;
}


void DrawLine(const vec3& origin, const vec3& destination) {
    __lazy_init();

    vec3 vertices[2] = { origin, destination} ;

    gl_program_->Use();
    gl_program_->SetUniformsForBuiltins();

    glUniform4f(color_location_, color_.r, color_.g, color_.b, color_.a);
    CHECK_GL_ERROR();

    glEnableVertexAttribArray(kVertexAttrib_Position);
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(kVertexAttrib_Position);
}

void DrawRect(const Rect& rect) {
    vec2 origin(rect.pos);
    vec2 dest(rect.pos + rect.size);

    DrawLine(origin, vec2(dest.x, origin.y));
    DrawLine(vec2(dest.x, origin.y), dest);
    DrawLine(dest, vec2(origin.x, dest.y));
    DrawLine(vec2(origin.x, dest.y), origin);
}

void DrawPoly(const vec2 *poli, unsigned int num_points, bool close_polygon) {
    __lazy_init();

    gl_program_->Use();
    gl_program_->SetUniformsForBuiltins();
    
    glUniform4f(color_location_, color_.r, color_.g, color_.b, color_.a);
    CHECK_GL_ERROR();

    glEnableVertexAttribArray(kVertexAttrib_Position);

    //vec2 vertices[num_points];
    //for (int i = 0; i < num_points; ++i) {
    //    vertices[i] = vec2(poli[i].x, poli[i].y);
    //}
    glVertexAttribPointer(kVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, poli);

    if (close_polygon) {
        glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)num_points);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)num_points);
    }
}

}

