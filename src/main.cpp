#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include <unistd.h>
#include <cmath>

#include "notifications.hpp"
#include "macros.hpp"
#include "mem.hpp"
#include "argsParser.hpp"

// #define IN_CODE_ARGS
// #define TEST_ARGS_PARSER

class formatter_t {
  struct custom_t {
    std::string prefix = "";
    std::string postfix = "";
    std::string valueColor = "";
    std::string prefixColor = "";
    std::string postfixColor = "";
    void reset(void) {
      this->postfix = "?";
      this->prefix = "?";
      this->valueColor.clear();
      this->valueColor.shrink_to_fit();
      this->prefixColor.clear();
      this->prefixColor.shrink_to_fit();
      this->postfixColor.clear();
      this->postfixColor.shrink_to_fit();
    }
  };
public:
  std::string value = "";
  uint32_t reserved_gaps = NULL;
  custom_t* custom = new custom_t();
  std::string getFormatStr(void) const {
    return (custom->prefixColor == "" ? "" : "${F" + custom->prefixColor + "}") + custom->prefix + 
    (custom->valueColor == "" ? "" : "${F" + custom->valueColor + "}") + 
    (value.size() < reserved_gaps ? std::string(reserved_gaps - value.size(), ' ') : "") + value + 
    (custom->postfixColor == "" ? "" : "${F" + custom->postfixColor + "}") + custom->postfix;
  }
  formatter_t() {
    custom->reset();
  }
  ~formatter_t() {
    delete custom;
  }
} formatter;



[[nodiscard, gnu::noinline]] extern auto
main(signed int argc, const char** argv) -> decltype(argc) {
  std::vector<std::string> argvs;
#ifdef IN_CODE_ARGS
  argvs = { "--value=RAM", };
#else
  
  if (argc == 1) { 
    //cout --help
    return EXIT_SUCCESS;
  }
  {
    uint32_t index = 1u;
    while (index < argc) {
      argvs.push_back( *(argv + index) );
      ++index;
    }
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
#ifdef TEST_ARGS_PARSER
  return EXIT_SUCCESS;
#endif 
  int32_t bestValue = INT32_MIN, value = INT32_MIN;
  size_t argIndex = NULL;
  bool canChange = true;
  for (const std::pair<std::string, std::string>& arg : parsedArgs) {
    ++argIndex;
    if (arg.second.size() > NULL) {
      if (arg.first == "value") {
        if (arg.second == "RAM") {
          mem_t mem;
          if (!getRAM(mem)) { continue; }
          value = std::round(((long double)mem.MemTotal - mem.MemFree) / mem.MemTotal * 100.0L);
          formatter.value = std::to_string(value);
        } else if (arg.second == "CPU") {
          
        } else {
          bool success;
          int32_t valueInArg = argValueToInt(arg, argIndex, success);
          if (!success) { continue; }
          value = valueInArg;
          formatter.value = std::to_string(value);
        }
      }
      if (arg.first == "reserved_gaps") {
        bool success;
        int32_t valueInArg = argValueToInt(arg, argIndex, success);
        if (valueInArg < NULL) {
          success = false;
          CENE_WITH_ARG(NE_4, arg, argIndex);
        }
        if (!success) { continue; }
        formatter.reserved_gaps = valueInArg;
      }
      if (arg.first == "new_part") {
        bool success;
        int32_t partValue = argValueToInt(arg, argIndex, success);
        if (!success) { continue; }
        if (partValue >= bestValue && value > partValue) {
          formatter.custom->reset();
          bestValue = partValue;
          canChange = true;
          continue;
        } else {
          canChange = false;
        }
      } // add here some co::error's
      if (canChange) {
        if (arg.first == "prefix") {
          formatter.custom->prefix = arg.second;
        } else if (arg.first == "postfix") {
          formatter.custom->postfix = arg.second;
        } else if (arg.first == "prefix_color") {
          formatter.custom->prefixColor = arg.second;
        } else if (arg.first == "postfix_color") {
          formatter.custom->postfixColor = arg.second;
        }
      }  
    }
  }
  printf("%s", formatter.getFormatStr().c_str()); ENDLN;
  return EXIT_SUCCESS;
}

#ifdef IN_CODE_ARGS
#undef IN_CODE_ARGS
#endif
#ifdef TEST_ARGS_PARSER
#undef TEST_ARGS_PARSER
#endif
