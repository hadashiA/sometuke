#ifndef __kawaii__color__
#define __kawaii__color__

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace kawaii {

struct Color3B {
public:
    Color3B() {}
    Color3B(GLubyte red, GLubyte green, GLubyte blue)
        : r(red), g(green), b(blue) {}

    GLubyte r;
    GLubyte g;
    GLubyte b;
};

struct Color4B {
public:
    Color4B() {}
    Color4B(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
        : r(red), g(green), b(blue), a(alpha) {}
    Color4B(Color3B color3, GLubyte alpha)
        : r(color3.r), g(color3.g), b(color3.b), a(alpha) {}

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct Color4F {
public:
    Color4F() {}
    Color4F(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
        : r(red), g(green), b(blue), a(alpha) {}

    explicit Color4F(Color3B c) : r(c.r/255.f), g(c.g/255.f), b(c.b/255.f), a(1.f) {};
    explicit Color4F(Color4B c) : r(c.r/255.f), g(c.g/255.f), b(c.b/255.f), a(c.a/255.f) {};

    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

// struct Color4B {
//     explicit Color4B(Color4F c) : r(c.r*255.f), g(c.g*255.f), b(c.b*255.f), a(c.a*255.f) {};
// };

}

#endif /* defined(__kawaii__color__) */
