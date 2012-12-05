#ifndef __kawaii__logger__
#define __kawaii__logger__

#include <string>

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#define IIINFO(s, ...)   kawaii::log("INFO", s, ##__VA_ARGS__)
#define IIWARN(s, ...)   kawaii::log("WARN", s, ##__VA_ARGS__)
#define IIERROR(s, ...)  kawaii::log("ERROR", s, ##__VA_ARGS__)

namespace kawaii {
using namespace std;

static void log(const char *loglevel, const string message, ...) {
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

}

#endif  /* defined(__kawaii__logger__) */
