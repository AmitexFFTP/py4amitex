#ifndef _AMITEX_BYTESWAP_HEADER_
#define _AMITEX_BYTESWAP_HEADER_

#include <cstdlib>

//! \file byteswap.h
//! Uses a general implementation and/or compiler intrinsics

template <typename T>
static inline T byteswap(T value) noexcept {
  static_assert(std::is_fundamental_v<T>);
  constexpr size_t n = sizeof(T);
  std::array<unsigned char, n> value_representation;
  std::memcpy(&value_representation, &value, n);
  for (size_t i = 0; i < n / 2; i++) {
    std::swap(value_representation[i], value_representation[n - i - 1]);
  }
  std::memcpy(&value, &value_representation, n);
  return value;
}

#if defined(__GNUC__) or defined(__clang__) or defined(__INTEL_COMPILER)
template <>
inline int32_t byteswap(int32_t value) noexcept {
  return __builtin_bswap32(value);
}
template <>
inline int64_t byteswap(int64_t value) noexcept {
  return __builtin_bswap64(value);
}
template <>
inline double byteswap(double value) noexcept {
  union {
    double d;
    uint64_t u;
  } evalue, retval;
  evalue.d = value;
  retval.u = __builtin_bswap64(evalue.u);
  return retval.d;
}
static inline double byteswap(float value) {
  union {
    float d;
    uint32_t u;
  } evalue, retval;
  evalue.d = value;
  retval.u = __builtin_bswap32(evalue.u);
  return retval.d;
}
#endif

#endif  // _AMITEX_BYTESWAP_HEADER_
