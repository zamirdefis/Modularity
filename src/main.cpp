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

class formatter_t final {
  class custom_t final {
  private:
    std::string prefix_ = "";
    std::string postfix_ = "";
    std::string valueColor_ = "";
    std::string prefixColor_ = "";
    std::string postfixColor_ = "";
  public:
    void reset(void) {
      postfix_ = "?";
      prefix_ = "?";
      valueColor_.clear();
      valueColor_.shrink_to_fit();
      prefixColor_.clear();
      prefixColor_.shrink_to_fit();
      postfixColor_.clear();
      postfixColor_.shrink_to_fit();
    }
    void setPrefix(std::string_view prefix) {
      prefix_ = prefix;
    }
    void setPostfix(std::string_view postfix) {
      postfix_ = postfix;
    }
    void setValueColor(std::string_view valueColor) {
      valueColor_ = valueColor;
    }
    void setPrefixColor(std::string_view prefixColor) {
      prefixColor_ = prefixColor;
    }
    void setPostfixColor(std::string_view postfixColor) {
      postfixColor_ = postfixColor;
    }
    
    const std::string& getPrefix(void) {
      return prefix_;
    }
    const std::string& getPostfix(void) {
      return postfix_;
    }
    const std::string& getValueColor(void) {
      return valueColor_;
    }
    const std::string& getPrefixColor(void) {
      return prefixColor_;
    }
    const std::string& getPostfixColor(void) {
      return postfixColor_;
    }
  };
public:
  std::string value = "";
  uint32_t reserved_gaps = NULL;
  custom_t* custom = new custom_t();
  std::string getFormatStr(void) const {
    return (custom->getPrefixColor() == "" ? "" : "${F" + custom->getPrefixColor() + "}") + custom->getPrefix() + 
    (custom->getValueColor() == "" ? "" : "${F" + custom->getValueColor() + "}") + 
    (value.size() < reserved_gaps ? std::string(reserved_gaps - value.size(), ' ') : "") + value + 
    (custom->getPostfixColor() == "" ? "" : "${F" + custom->getPostfixColor() + "}") + custom->getPostfix();
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
  std::vector<arg_t> parsedArgs;
  for (const std::string& fullArg : argvs) {
    auto parsedArg = parseArg(fullArg);
    if (parsedArg.size() == NULL) {
      co::error(static_cast<std::string>(NE_2) + " : \"" + fullArg + "\"");
      return EXIT_FAILURE;
    }
    for (const arg_t& arg : parsedArg) {
      parsedArgs.push_back(arg);
    }
  }
  int32_t bestValue = INT32_MIN, value = INT32_MIN;
  size_t argIndex = NULL;
  bool canChange = true;
  for (const arg_t& arg : parsedArgs) {
    ++argIndex;
    if (arg.value.size() > NULL) {
      if (arg.main == "value") {
        if (arg.value == "RAM") {
          mem_t mem;
          if (!getRAM(mem)) { continue; }
          value = std::round(((long double)mem.MemTotal - mem.MemFree) / mem.MemTotal * 100.0L);
          formatter.value = std::to_string(value);
        } else if (arg.value == "CPU") {
          
        } else {
          bool success;
          int32_t valueInArg = argValueToInt(arg, argIndex, success);
          if (!success) { continue; }
          value = valueInArg;
          formatter.value = std::to_string(value);
        }
      }
      if (arg.main == "reserved_gaps") {
        bool success;
        int32_t valueInArg = argValueToInt(arg, argIndex, success);
        if (valueInArg < NULL) {
          success = false;
          CENE_WITH_ARG(NE_4, arg, argIndex);
        }
        if (!success) { continue; }
        formatter.reserved_gaps = valueInArg;
      }
      if (arg.main == "new_part") {
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
        if (arg.main == "prefix") {
          formatter.custom->setPrefix(arg.value);
        } else if (arg.main == "postfix") {
          formatter.custom->setPostfix(arg.value);
        } else if (arg.main == "prefix_color") {
          formatter.custom->setPrefixColor(arg.value);
        } else if (arg.main == "postfix_color") {
          formatter.custom->setPostfixColor(arg.value);
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
