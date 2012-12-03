#ifndef __kawaii__logger__
#define __kawaii__logger__

#include <string>

#define IIINFO(s, ...)   kawaii::log("INFO", s, ##__VA_ARGS__)
#define IIWARN(s, ...)   kawaii::log("WARN", s, ##__VA_ARGS__)
#define IIERROR(s, ...)  kawaii::log("ERROR", s, ##__VA_ARGS__)

namespace kawaii {
using namespace std;

void log(const char *loglevel, const string message, ...);
}

#endif  /* defined(__kawaii__logger__) */
