#ifndef FUNSPLIT_UTILS_H
#define FUNSPLIT_UTILS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

namespace fs {

template < typename ENUMDEST, typename ENUMORIG>
ENUMDEST EnumConversion(const ENUMORIG  & value)
{
  return static_cast<ENUMDEST>(static_cast<typename std::underlying_type<ENUMORIG>::type>(value));
}

inline void 
PrintBuffer(const char* buf, size_t buf_len) // Method to print the hexas of buf
{
  size_t i = 0;
  for (i = 0; i < buf_len; ++i) {
    fprintf(stdout, "%02X%s", (unsigned char)buf[i], (i + 1) % 16 == 0 ? "\r\n" : " ");
  }
  fprintf(stdout, "\n\n");
}

inline void 
PrintBuffer(const std::string& str) // Method to print the hexas of buf
{
  const char* buf = str.c_str();
  auto buf_len = str.size();
  std::cout << "Size of the string " << buf_len << std::endl;
  size_t i = 0;
  for (i = 0; i < buf_len; ++i) {
    fprintf(stdout, "%02X%s", (unsigned char)buf[i], (i + 1) % 16 == 0 ? "\r\n" : " ");
  }
  fprintf(stdout, "\n\n");
}
}

#endif /* FUNSPLIT_UTILS_H */

