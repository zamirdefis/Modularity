#include <stdio.h>
#include <stdlib.h>
#include <macros.hpp>

[[nodiscard, gnu::noinline]] extern auto
main(signed int argc, const char** argv) -> decltype(argc) {
  printf("HeLLOwOrld! >_<"); ENDLN;
  return EXIT_SUCCESS;
}
