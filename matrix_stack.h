#ifndef __kawaii__matrix_stack__
#define __kawaii__matrix_stack__

#include <vector>
#include <memory>
#include "matrix.h"

namespace kawaii {
using namespace std;

class MatrixStack {
public:
    static MatrixStack& GLProjection() {
        static unique_ptr<MatrixStack> __gl_projection(new MatrixStack);
        return *__gl_projection;
    }

    static MatrixStack& GLModelView() {
        static unique_ptr<MatrixStack> __gl_model_view(new MatrixStack);
        return* __gl_model_view;
    }

    static MatrixStack& GLTexture() {
        static unique_ptr<MatrixStack> __gl_texture(new MatrixStack);
        return *__gl_texture;
    }

    mat4& Push() {
        stack_.push_back(stack_.empty() ? mat4::Identity() : Top());
        return stack_.back();
    }

    mat4 Pop() {
        mat4 result = stack_.back();
        stack_.pop_back();
        return result;
    }

    mat4& Top() {
        return stack_.back();
    }

private:
    MatrixStack() : stack_() {
        stack_.push_back(mat4::Identity());
    }

    vector<mat4> stack_;
};

}

#endif /* defined(__kawaii__matrix_stack__) */
