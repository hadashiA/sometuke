#ifndef __skidarake_vertices__
#define __skidarake_vertices__

#include "skidarake/vector.h"
#include "skidarake/color.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace skidarake {

typedef Vector2<GLfloat> vec2gl;
typedef Vector3<GLfloat> vec3gl;

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
    vec2gl pos;                   // 8 bytes
    Color4B color;              // 4 bytes
    GLfloat size;               // 4 bytes
};

// A 2D Quad. 4 * 2 floats
struct Quad2 {
    vec2gl tl;
    vec2gl tr;
    vec2gl bl;
    vec2gl br;
};

// A 3D Quad. 4 * 3 floats
struct Quad3 {
    vec3gl bl;
    vec3gl br;
    vec3gl tl;
    vec3gl tr;
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P2F_C4B_T2F {
    vec2gl pos;                   // 2F
    Color4B color;              // 4B
    Tex2F tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P2F_C4F_T2F {
    vec2gl pos;                    // 2F
    Color4F color;               // 4F
    Tex2F tex_coord;              // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct P3F_C4F_T2F {
    vec3gl pos;                   // 3F
    Color4F color;              // 4F
    Tex2F tex_coord;             // 2F
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct P3F_C4B_T2F {
    vec3gl pos;                   // 3F
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

#endif /* defined(__skidarake_vertices_) */
