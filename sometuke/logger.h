#ifndef __sometuke__logger__
#define __sometuke__logger__

#include "sometuke/vector.h"
#include "sometuke/matrix.h"
#include "sometuke/vertices.h"
#include "sometuke/hashed_string.h"
#include "sometuke/sprite_frame.h"

#include <string>
#include <sstream>

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#if !defined(SOMETUKE_DEBUG) || SOMETUKE_DEBUG == 0
#define S2INFO(...)  do {} while (0)
#define IIWARN(...)  do {} while (0)
#define S2ERROR(s, ...) sometuke::Log("ERROR", __FILE__, __LINE__, s, ##__VA_ARGS__)

#elif SOMETUKE_DEBUG == 1
#define S2INFO(...)     do {} while (0)
#define IIWARN(s, ...)  sometuke::Log("WARN", __FILE__, __LINE__, s, ##__VA_ARGS__)
#define S2ERROR(s, ...) sometuke::Log("ERROR", __FILE__, __LINE__, s, ##__VA_ARGS__)

#elif SOMETUKE_DEBUG > 1
#define S2INFO(s, ...)  sometuke::Log("INFO", __FILE__, __LINE__, s, ##__VA_ARGS__)
#define IIWARN(s, ...)  sometuke::Log("WARN", __FILE__, __LINE__, s, ##__VA_ARGS__)
#define S2ERROR(s, ...) sometuke::Log("ERROR", __FILE__, __LINE__, s, ##__VA_ARGS__)
#endif

#define S2INSPECT(obj) sometuke::Inspect(obj).c_str()

namespace sometuke {
using namespace std;

static inline void Log(const char *loglevel,
                       const char *file,
                       unsigned int line,
                       const string message, ...) {
    time_t now = time(NULL);
    struct tm *ts = localtime(&now);
    printf("%s:%d - %04d-%02d-%02d %02d:%02d:%02d [%s] ",
           file,
           line,
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

#endif  /* defined(__sometuke__logger__) */
