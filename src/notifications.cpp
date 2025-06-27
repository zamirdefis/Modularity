#include "notifications.hpp"
#include "macros.hpp"

extern void co::error(const std::string& str) {
  printf("%s %s", ERROR_PREFIX, str.c_str()); ENDLN;
}
