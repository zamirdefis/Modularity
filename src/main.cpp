#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>
#include <queue>

#include "notifications.hpp"
#include "macros.hpp"

// #define IN_CODE_ARGS
// #define TEST_ARGS_PARSER

enum class ArgType : uint8_t {
  Invalid,
  Brief,
  Single,
  Value,
};

[[nodiscard]] static ArgType getArgType(std::string_view fullArg) {
  size_t realPartIndex;
  // printf("%d", (fullArg.size() < 2u || fullArg.front() != '-' 
  // || (realPartIndex = fullArg.find_first_not_of('-'), printf("rpi : %zu\n", realPartIndex), realPartIndex) == SIZE_MAX) ||
  // !std::isalpha(fullArg[realPartIndex])); ENDLN;
  if (fullArg.size() < 2u || fullArg.front() != '-' ||
    (realPartIndex = fullArg.find_first_not_of('-'), realPartIndex) == SIZE_MAX ||
    fullArg[realPartIndex] == '=') {
    return ArgType::Invalid;
  }
  const decltype(realPartIndex) equal = fullArg.find('=');
  if (fullArg[1u] == '-') {
    if (equal == SIZE_MAX) {
      return ArgType::Single;
    }
    return ArgType::Value;
  }
  if (equal != SIZE_MAX) {
    return ArgType::Invalid;
  }
  return ArgType::Brief;
}

[[nodiscard]] static std::vector<std::pair<std::string, std::string>> parseArg(const std::string& fullArg) {
  std::vector<std::pair<std::string, std::string>> result{};
  if (ArgType argInfo = getArgType(fullArg); argInfo == ArgType::Invalid) {
    co::error(static_cast<std::string>(NE_1) + " : \"" + fullArg + "\"");
    return result; } else if (argInfo == ArgType::Brief) {
    for (std::string::const_iterator citer = fullArg.cbegin() + 1u;
    citer != fullArg.cend(); ++citer) {
      result.push_back(std::make_pair(std::string(1, *citer), ""));
    }
    return result;
  } else if (argInfo == ArgType::Value) {
    const size_t equalIndex = fullArg.find('=');
    return {std::make_pair(fullArg.substr(2u, equalIndex - 2u),
    fullArg.substr(equalIndex + 1u, fullArg.size() - (equalIndex + 1u)))};
  }
  return {std::make_pair(fullArg.substr(2u, fullArg.size() - 2u), "")};
}

[[nodiscard, gnu::noinline]] extern auto
main(signed int argc, const char** argv) -> decltype(argc) {
  std::vector<std::string> argvs;
#ifdef IN_CODE_ARGS
  argvs = {"--z=l", "--kkkk=1234", "--o=", "--help", "--H", "--", "---kkk", "-h", "", "-", "-=", "-a=z"};
#else
  
  if (argc == 1) { 
    //cout --help
    return EXIT_SUCCESS;
  }
  uint32_t index = 1u;
  while (index < argc) {
    argvs.push_back( *(argv + index) );
    ++index;
  }
#endif
  std::vector<std::pair<std::string, std::string>> parsedArgs;
  for (const std::string& fullArg : argvs) {
    auto parsedArg = parseArg(fullArg);
    if (parsedArg.size() == NULL) {
      co::error(static_cast<std::string>(NE_2) + " : \"" + fullArg + "\"");
#ifdef TEST_ARGS_PARSER
      continue;
#else
      return EXIT_FAILURE;
#endif
    }
    for (const std::pair<std::string, std::string>& el : parsedArg) {
#ifdef TEST_ARGS_PARSER
      printf("%s:%s", el.first.c_str(), el.second.c_str()); ENDLN;
#else
      parsedArgs.push_back(el);
#endif
    }
  }
  // void (*(*pfa[10])(int))(int);
  using argType = std::pair<std::string, std::string>;
  using partsType = std::pair<int32_t, std::vector<argType>>;
  std::priority_queue<partsType, std::vector<partsType>, auto(*)(const partsType&, const partsType&)->bool> parts{ [](const partsType& a, const partsType& b){ return a.first < b.first; } };
  partsType tmp{INT32_MAX,};
  for (std::pair<std::string, std::string> arg : parsedArgs) {
    if (arg.first == "new_part") {
      
    }
  }
  return EXIT_SUCCESS;
}

#ifdef IN_CODE_ARGS
#undef IN_CODE_ARGS
#endif
#ifdef TEST_ARGS_PARSER
#undef TEST_ARGS_PARSER
#endif
