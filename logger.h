#ifndef __kawaii__logger__
#define __kawaii__logger__

#include "kawaii/vector.h"
#include "kawaii/matrix.h"
#include "kawaii/vertices.h"
#include "kawaii/hashed_string.h"
#include "kawaii/sprite_frame.h"

#include <string>
#include <sstream>

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#if !defined(KAWAII_DEBUG) || KAWAII_DEBUG == 0
#define IIINFO(...)  do {} while (0)
#define IIWARN(...)  do {} while (0)
#define IIERROR(s, ...) kawaii::Log("ERROR", s, ##__VA_ARGS__)

#elif KAWAII_DEBUG == 1
#define IIINFO(...)     do {} while (0)
#define IIWARN(s, ...)  kawaii::Log("WARN", s, ##__VA_ARGS__)
#define IIERROR(s, ...) kawaii::Log("ERROR", s, ##__VA_ARGS__)

#elif KAWAII_DEBUG > 1
#define IIINFO(s, ...)  kawaii::Log("INFO", s, ##__VA_ARGS__)
#define IIWARN(s, ...)  kawaii::Log("WARN", s, ##__VA_ARGS__)
#define IIERROR(s, ...) kawaii::Log("ERROR", s, ##__VA_ARGS__)
#endif

#define IIINSPECT(obj) kawaii::Inspect(obj).c_str()

namespace kawaii {
using namespace std;

static inline void Log(const char *loglevel, const string message, ...) {
    time_t now = time(NULL);
    struct tm *ts = localtime(&now);
    printf("kawaii - %04d-%02d-%02d %02d:%02d:%02d [%s] ",
           ts->tm_year + 1900,
           ts->tm_mon + 1,
           ts->tm_mday,
           ts->tm_hour,
           ts->tm_min,
           ts->tm_sec,
           loglevel
           );

    va_list args;
    va_start(args, message);
    vprintf(message.c_str(), args);
    va_end(args);

    printf("\n");
}

// vector stuff
static inline string Inspect(const string& str) {
    return str;
}

static inline string Inspect(const vec2& v) {
    stringstream ss;
    ss << "{x:" << v.x << ", y:" << v.y << "}";
    return ss.str();
}

static inline string Inspect(const ivec2& v) {
    stringstream ss;
    ss << "{x:" << v.x << ",y: " << v.y << "}";
    return ss.str();
}

static inline string Inspect(const vec3& v) {
    stringstream ss;
    ss << "{x:" << v.x << ", y:" << v.y << ", z:" << v.z << "}";
    return ss.str();
}

static inline string Inspect(const Rect& rect) {
    stringstream ss;
    ss << "{x:" << rect.pos.x << ", y:" << rect.pos.y <<
        ", w:" << rect.size.x << ", h:" << rect.size.y << "}";
    return ss.str();
}

// color stuff
static inline string Inspect(const Color3B& color) {
    stringstream ss;
    ss << "{r:" << color.r << ", g:" << color.g << ", b:" << color.b << "}";
    return ss.str();
}

static inline string Inspect(const Color4B& color) {
    stringstream ss;
    ss << "{r:" << color.r << ", g:" << color.g << ", b:" << color.b <<
        ", a:" << color.a << "}";
    return ss.str();
}

static inline string Inspect(const Color4F& color) {
    stringstream ss;
    ss << "{r:" << color.r << ", g:" << color.g << ", b:" << color.b <<
        ", a:" << color.a << "}";
    return ss.str();
}

// vertices stuff
static inline string Inspect(const Tex2F& tex) {
    stringstream ss;
    ss << "{u:" << tex.u << ", v:" << tex.v << "}";
    return ss.str();
}

static inline string Inspect(const P3F_C4B_T2F& vertex) {
    stringstream ss;
    ss <<
        "{pos:" << Inspect(vertex.pos) <<
        ", color:" << Inspect(vertex.color) <<
        ", tex_coord:" << Inspect(vertex.tex_coord) <<
        "}";

    return ss.str();
}

static inline string Inspect(const P3F_C4F_T2F& vertex) {
    stringstream ss;
    ss << "{pos:" << Inspect(vertex.pos) <<
        ", color:" << Inspect(vertex.color) <<
        ", tex_coord: " << Inspect(vertex.tex_coord) <<
        "}";

    return ss.str();
}

static inline string Inspect(const P3F_C4B_T2F_Quad& quad) {
    stringstream ss;

    ss <<
        "{tl:"      << Inspect(quad.tl) <<
        ", bl:"  << Inspect(quad.bl) <<
        ", tl:"     << Inspect(quad.tl) <<
        ", borrom_right:" << Inspect(quad.br) << 
        "}";

    return ss.str();
}

static inline string Inspect(const HashedString& hashed_string) {
    stringstream ss;
    ss << "{string:" << hashed_string.str() << ", id:" << hashed_string.id() << "}";
    return ss.str();
}

static inline string Inspect(const mat4& m) {
    stringstream ss;
    ss << 
        m.x.x << ", " << m.x.y << ", " << m.x.z << ", " << m.x.w << "\n" <<
        m.y.x << ", " << m.y.y << ", " << m.y.z << ", " << m.y.w << "\n" <<
        m.z.x << ", " << m.z.y << ", " << m.z.z << ", " << m.z.w << "\n" <<
        m.w.x << ", " << m.w.y << ", " << m.w.z << ", " << m.w.w;
    return ss.str();
}

static inline string Inspect(bool value) {
    return value ? "true" : "false";
}

static inline string Inspect(const SpriteFrame& v) {
    stringstream ss;
    ss <<
        "{rect:" << Inspect(v.rect) <<
        ", rotated:" << Inspect(v.rotated) <<
        ", original_size:" << Inspect(v.original_size) <<
        ", name:\"" << v.name.str() << "\"" <<
        "}";
    return ss.str();
}

}

#endif  /* defined(__kawaii__logger__) */
