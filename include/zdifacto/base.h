#ifndef ZDIFACTO_BASE_H_
#define ZDIFACTO_BASE_H_
#include <vector>
#include <string>

#include "dmlc/logging.h"

namespace zdifacto {

  typedef float real_t;

  typedef uint64_t feaid_t;

  typedef std::vector<std::pair<std::string, std::string>> KWArgs;

  inline char* GetRole() { return getenv("DMLC_ROLE"); }

  inline bool IsDistributed() { return GetRole() != nullptr; }

  #define DEFAULT_NTHREADS 2

  #ifndef REVERSE_FEATURE_ID
  #define REVERSE_FEATURE_ID 0
  #endif
  inline feaid_t ReverseBytes(feaid_t x) {
  #if REVERSE_FEATURE_ID
    // return x;
    x = x << 32 | x >> 32;
    x = (x & 0x0000FFFF0000FFFFULL) << 16 |
        (x & 0xFFFF0000FFFF0000ULL) >> 16;
    x = (x & 0x00FF00FF00FF00FFULL) << 8 |
        (x & 0xFF00FF00FF00FF00ULL) >> 8;
    x = (x & 0x0F0F0F0F0F0F0F0FULL) << 4 |
        (x & 0xF0F0F0F0F0F0F0F0ULL) >> 4;
  #endif
    return x;
  }
}

#endif