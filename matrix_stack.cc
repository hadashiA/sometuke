#include "matrix_stack.h"

namespace kawaii {

MatrixStack *MatrixStack::__gl_projection = NULL;
MatrixStack *MatrixStack::__gl_model_view = NULL;
MatrixStack *MatrixStack::__gl_texture    = NULL;


void MatrixStack::Purge() {
    if (__gl_projection != NULL) {
        delete __gl_projection;
        __gl_projection = NULL;
    }

    if (__gl_model_view) {
        delete __gl_model_view;
        __gl_model_view = NULL;
    }

    if (__gl_texture) {
        delete __gl_texture;
        __gl_texture = NULL;
    }
}

}
