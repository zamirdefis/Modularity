#include <stdio.h>
#include <stdlib.h>
#include <macros.hpp>

[[nodiscard]] extern auto main(int argc, char* argv[]) ->decltype(argc) {
  printf("HeLLOwOrld! >_<"); ENDLN;
  return EXIT_SUCCESS;
}
