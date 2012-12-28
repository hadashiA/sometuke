#ifndef __kawaii__logger__
#define __kawaii__logger__

#include "vector.h"
#include "matrix.h"
#include "vertices.h"

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

static void Log(const char *loglevel, const string message, ...) {
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

static string Inspect(const string& str) {
    return str;
}

static string Inspect(const vec2& v) {
    stringstream ss;
    ss << "vec2(";
    ss << v.x;
    ss << ",";
    ss << v.y;
    ss << ")";
    return ss.str();
}

static string Inspect(const ivec2& v) {
    stringstream ss;
    ss << "ivec2(";
    ss << v.x;
    ss << ", ";
    ss << v.y;
    ss << ")";
    return ss.str();
}

static string Inspect(const Rect& rect) {
    stringstream ss;
    ss << "Rect(";
    ss << rect.pos.x;
    ss << ",";
    ss << rect.pos.y;
    ss << " ";
    ss << rect.size.x;
    ss << "/";
    ss << rect.size.y;
    ss << ")";
    return ss.str();
}

}

#endif  /* defined(__kawaii__logger__) */
