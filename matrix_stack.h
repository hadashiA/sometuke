#ifndef __kawaii__matrix_stack__
#define __kawaii__matrix_stack__

#include <vector>
#include <memory>
#include "matrix.h"

namespace kawaii {
using namespace std;

class MatrixStack {
public:
    static MatrixStack *GLProjection();
    static MatrixStack *GLModelView();
    static MatrixStack *GLTexture();
    static void Purge();

    void Push(const mat4& matrix);
    mat4 Pop();
    mat4 Get() const;

private:
    MatrixStack() : stack_() {
        stack_.push_back(mat4::Identity());
    }

    static MatrixStack *__gl_projection;
    static MatrixStack *__gl_model_view;
    static MatrixStack *__gl_texture;

    vector<mat4> stack_;
};

}

#endif /* defined(__kawaii__matrix_stack__) */
