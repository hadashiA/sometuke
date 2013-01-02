#include "hashed_string.h"

namespace kawaii {

void *HashedString::HashName(const char *ident_c_str) {
    // 65536未満で最大の素数
    const unsigned long BASE = 65521L;

    // NMAXは、255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1を満たすnの最大値
    const unsigned long NMAX = 5552;

#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

    if (ident_c_str == NULL)
        return NULL;

    unsigned long s1 = 0;
    unsigned long s2 = 0;
    
    for (size_t len=strlen(ident_c_str); len > 0;) {
        unsigned long k = len < NMAX ? len : NMAX;

        len -= k;
        
        while (k >= 16) {
            DO16(ident_c_str);
            ident_c_str += 16;
            k -= 16;
        }
		
        if (k != 0)
            do {
                s1 += tolower(*ident_c_str++);
                s2 += s1;
            } while (--k);
		
        s1 %= BASE;
        s2 %= BASE;
    }

#pragma warning(push)
#pragma warning(disable : 4312)

    return reinterpret_cast<void *>( (s2 << 16) | s1 );

#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}

}
