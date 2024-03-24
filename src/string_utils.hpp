#ifndef __STRING_UTILS_HPP__
#define __STRING_UTILS_HPP__

#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <cstdio>

inline void to_s_str_hhmmss(char* out, size_t size, int64_t s)
{
    int ss = s%60;
    int mm = (s/60)%60;
    int hh = s/3600;
    snprintf(out, size, "%02d:%02d:%02d", hh, mm, ss);
    out[size-1] = 0;
}

#endif // __STRING_UTILS_HPP__