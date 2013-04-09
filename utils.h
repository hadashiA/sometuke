#ifndef __kawaii__utils__
#define __kawaii__utils__

#include <uuid/uuid.h>
#include <string.h>

namespace kawaii {

inline unsigned long generate_id() {
    unsigned long number;

    uuid_t uuid;
    uuid_generate(uuid);
    memcpy(&number, uuid, sizeof(number));

    return number;
}

}

#endif /* defined(__kawaii__utils) */
