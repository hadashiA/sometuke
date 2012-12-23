#ifndef __kawaii__color__
#define __kawaii__color__

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace kawaii {

struct Color3B {
public:
    Glubyte r;
    GLubyte g;
    GLubyte b;
};

struct Color4B {
public:
    explicit Color4B(Color4F c) : r(c.r*255.f, c.g*255.f, c.b*255.f, c.a*255.f) {};

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct Color4F {
public:
    explicit Color4F(Color3B c) : r(c.r/255.f, c.g/255.f, c.b/255.f, 1.f) {};
    explicit Color4F(Color4B c) : r(c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f) {};

    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

}

#endif /* defined(__kawaii__color__) */
