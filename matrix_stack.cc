#include "matrix_stack.h"

namespace kawaii {

MatrixStack *MatrixStack::__gl_projection = NULL;
MatrixStack *MatrixStack::__gl_model_view = NULL;
MatrixStack *MatrixStack::__gl_texture    = NULL;

MatrixStack *MatrixStack::GLProjection() {
    if (__gl_projection == NULL) {
        __gl_projection = new MatrixStack();
    }
    return __gl_projection;
}

MatrixStack *MatrixStack::GLModelView() {
    if (__gl_model_view == NULL) {
        __gl_model_view = new MatrixStack();
    }
    return __gl_model_view;
}

MatrixStack *MatrixStack::GLTexture() {
    if (__gl_texture == NULL) {
        __gl_texture = new MatrixStack();
    }
    return __gl_texture;
}

void MatrixStack::Purge() {
    delete __gl_projection;
    __gl_projection = NULL;

    delete __gl_model_view;
    __gl_model_view = NULL;

    delete __gl_texture;
    __gl_texture = NULL;
}

void MatrixStack::Push(const mat4& matrix) {
    stack_.push_back(matrix);
}

mat4 MatrixStack::Pop() {
    mat4 result = stack_.back();
    stack_.pop_back();

    return result;
}

mat4 MatrixStack::Get() const {
    return stack_.back();
}

}
