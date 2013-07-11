#ifndef __skidarake__matrix_stack__
#define __skidarake__matrix_stack__

#include <stack>
#include <memory>
#include "skidarake/matrix.h"

namespace skidarake {
using namespace std;

struct GLProjection {};
struct GLModelView {};
struct GLTexture {};

template <typename T>
class MatrixStack {
public:
    static MatrixStack& Instance() {
        static unique_ptr<MatrixStack<T> > __instance(new MatrixStack);
        return *__instance;
    }

    mat4& Push() {
        stack_.push(stack_.empty() ? mat4::Identity() : Top());
        return stack_.top();
    }

    mat4 Pop() {
        mat4 result = stack_.top();
        stack_.pop();
        return result;
    }

    mat4& Top() {
        return stack_.top();
    }

private:
    MatrixStack() : stack_() {
        stack_.push(mat4::Identity());
    }

    MatrixStack(const MatrixStack&);
    MatrixStack& operator=(const MatrixStack&);

    stack<mat4> stack_;
};

}

#endif /* defined(__skidarake__matrix_stack__) */
