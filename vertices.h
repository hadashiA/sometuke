#ifndef __kawaii_vertices__
#define __kawaii_vertices__

#include "kawaii/vector.h"
#include "kawaii/color.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace kawaii {

// XXX: If any of these enums are edited and/or reordered, update CCTexture2D.m
//! Vertical text alignment type
typedef enum {
    kVerticalTextAlignmentTop,
    kVerticalTextAlignmentCenter,
    kVerticalTextAlignmentBottom,
} VerticalTextAlignment;

// XXX: If any of these enums are edited and/or reordered, update CCTexture2D.m
//! Horizontal text alignment type
typedef enum {
    kTextAlignmentLeft,
    kTextAlignmentCenter,
    kTextAlignmentRight,
} TextAlignment;

struct Tex2F {
    GLfloat u;
    GLfloat v;
};

struct PointSprite {
    vec2 pos;                   // 8 bytes
    Color4B color;              // 4 bytes
    GLfloat size;               // 4 bytes
};

// A 2D Quad. 4 * 2 floats
struct Quad2 {
    vec2 tl;
    vec2 tr;
    vec2 bl;
    vec2 br;
};

// A 3D Quad. 4 * 3 floats
struct Quad3 {
    vec3 bl;
    vec3 br;
    vec3 tl;
    vec3 tr;
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P2F_C4B_T2F {
    vec2 pos;                   // 2F
    Color4B color;              // 4B
    Tex2F tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P2F_C4F_T2F {
    vec2 pos;                    // 2F
    Color4F color;               // 4F
    Tex2F tex_coord;              // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P3F_C4F_T2F {
    vec3 pos;                   // 3F
    Color4F color;              // 4F
    Tex2F tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P3F_C4B_T2F {
    vec3 pos;                   // 3F
    Color4B color;              // 4B
    Tex2F tex_coord;             // 2F
};


struct P2F_C4B_T2F_Triangle {
    P2F_C4B_T2F a;
    P2F_C4B_T2F b;
    P2F_C4B_T2F c;
};

struct P3F_C4F_T2F_Quad {
    P3F_C4F_T2F tl;
    P3F_C4F_T2F bl;
    P3F_C4F_T2F tr;
    P3F_C4F_T2F br;
};

struct P2F_C4B_T2F_Quad {
    P2F_C4B_T2F bl;
    P2F_C4B_T2F br;
    P2F_C4B_T2F tl;
    P2F_C4B_T2F tr;
};

struct P3F_C4B_T2F_Quad {
    P3F_C4B_T2F tl;
    P3F_C4B_T2F bl;
    P3F_C4B_T2F tr;
    P3F_C4B_T2F br;
};

struct P2F_C4F_T2F_Quad {
    P2F_C4F_T2F bl;
    P2F_C4F_T2F br;
    P2F_C4F_T2F tl;
    P2F_C4F_T2F tr;
};

}

#endif /* defined(__kawaii_vertices_) */
