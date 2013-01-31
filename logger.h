#ifndef __kawaii__logger__
#define __kawaii__logger__

#include "vector.h"
#include "matrix.h"
#include "vertices.h"
#include "hashed_string.h"

#include <string>
#include <sstream>

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#if !defined(KAWAII_DEBUG) || KAWAII_DEBUG == 0
#define IIINFO(...)  do {} while (0)
#define IIWARN(...)  do {} while (0)
#define IIERROR(...) do {} while (0)

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
    ss << "vec2(" << v.x << ", " << v.y << ")";
    return ss.str();
}

static inline string Inspect(const ivec2& v) {
    stringstream ss;
    ss << "ivec2(" << v.x << ", " << v.y << ")";
    return ss.str();
}

static inline string Inspect(const vec3& v) {
    stringstream ss;
    ss << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

static inline string Inspect(const Rect& rect) {
    stringstream ss;
    ss << "Rect(" << rect.pos.x << "," << rect.pos.y << " " <<
        rect.size.x << "/" << rect.size.y << ")";
    return ss.str();
}

// color stuff
static inline string Inspect(const Color3B& color) {
    stringstream ss;
    ss << "Color3B(" << color.r << ", " << color.g << ", " << color.b << ")";
    return ss.str();
}

static inline string Inspect(const Color4B& color) {
    stringstream ss;
    ss << "Color4B(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
    return ss.str();
}

static inline string Inspect(const Color4F& color) {
    stringstream ss;
    ss << "Color4F(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
    return ss.str();
}

// vertices stuff
static inline string Inspect(const Tex2F& tex) {
    stringstream ss;
    ss << "Tex2F(" << tex.u << ", " << tex.v << ")";
    return ss.str();
}

static inline string Inspect(const P3F_C4B_T2F& v) {
    stringstream ss;
    ss << "(" << Inspect(v.pos) << " " << Inspect(v.color) << " " << Inspect(v.tex_coord) << ")";

    return ss.str();
}

static inline string Inspect(const P3F_C4F_T2F& v) {
    stringstream ss;
    ss << "(" << Inspect(v.pos) << " " << Inspect(v.color) << " " << Inspect(v.tex_coord) << ")";

    return ss.str();
}

static inline string Inspect(const Quad_P3F_C4B_T2F& quad) {
    stringstream ss;

    ss << "(" <<
        "tl" << Inspect(quad.top_left) << " " <<
        "bl" << Inspect(quad.bottom_left) << " " <<
        "tl" << Inspect(quad.top_left) << " " <<
        "br" << Inspect(quad.bottom_right) << 
        ")";

    return ss.str();
}

static inline string Inspect(const HashedString& hashed_string) {
    stringstream ss;
    ss << "HashedString(" <<
        hashed_string.string() <<
        " -> " <<
        hashed_string.value() <<
        ")";
    return ss.str();
}

static inline string Inspect(const mat4& m) {
    stringstream ss;
    ss << "\n" << 
        m.x.x << ", " << m.x.y << ", " << m.x.z << ", " << m.x.w << "\n" <<
        m.y.x << ", " << m.y.y << ", " << m.y.z << ", " << m.y.w << "\n" <<
        m.z.x << ", " << m.z.y << ", " << m.z.z << ", " << m.z.w << "\n" <<
        m.w.x << ", " << m.w.y << ", " << m.w.z << ", " << m.w.w;
    return ss.str();
}

}

#endif  /* defined(__kawaii__logger__) */
