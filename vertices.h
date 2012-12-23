#ifndef __kawaii_vertices__
#define __kawaii_vertices__

#include "color.h"

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
    vec2 top_left;
    vec2 top_right;
    vec2 bottom_left;
    vec2 bottom_right;
};

// A 3D Quad. 4 * 3 floats
struct Quad3 {
    vec3 bottom_left;
    vec3 bottom_right;
    vec3 top_left;
    vec3 top_right;
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P2F_C4B_T2F {
    vec2 pos;                   // 2F
    Color4B color;              // 4B
    vec2 tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P2F_C4F_T2F {
    vec2 pos;                    // 2F
    Color4F color;               // 4F
    vec2 tex_coord;              // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P3F_C4F_T2F {
    vec3 pos;                   // 3F
    Color4F color;              // 4F
    vec2 tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P3F_C4B_T2F {
    vec3 pos;                   // 3F
    Color4B color;              // 4B
    vec2 tex_coord;             // 2F
};


struct Triangle_P2F_C4B_T2F {
    P2F_C4B_T2F a;
    P2F_C4B_T2F b;
    P2F_C4B_T2F c;
};

struct Quad_P3F_C4F_T2F {
    P3F_C4F_T2F top_left;
    P3F_C4F_T2F bottom_left;
    P3F_C4F_T2F top_right;
    P3F_C4F_T2F bottom_right;
};

struct Quad_P2F_C4B_T2F {
    P2F_C4B_T2F bottom_left;
    P2F_C4B_T2F bottom_right;
    P2F_C4B_T2F top_left;
    P2F_C4B_T2F top_right;
};

struct Quad_P3F_C4B_T2F {
    P3F_C4B_T2F top_left;
    P3F_C4B_T2F bottom_left;
    P3F_C4B_T2F top_right;
    P3F_C4B_T2F bottom_right;
};

struct Quad_P2F_C4F_T2F {
    P2F_C4F_T2F bottom_left;
    P2F_C4F_T2F bottom_right;
    P2F_C4F_T2F top_left;
    P2F_C4F_T2F top_right;
};

}

#endif /* defined(__kawaii_vertices_) */
