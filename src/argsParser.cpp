#include "argsParser.hpp"
#include "notifications.hpp"

#include <climits>

arg_t::arg_t() : main(""), value("") {}
arg_t::arg_t(std::string_view main) : main(main), value("")  {}
arg_t::arg_t(std::string_view main, std::string_view value) : main(main), value(value) {}

[[nodiscard]] ArgType getArgType(std::string_view fullArg) {
  size_t realPartIndex;
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

[[nodiscard]] std::vector<arg_t> parseArg(const std::string& fullArg) {
  std::vector<arg_t> result{};
  if (ArgType argInfo = getArgType(fullArg); argInfo == ArgType::Invalid) {
    co::error(static_cast<std::string>(NE_1) + " : \"" + fullArg + "\"");
    return result; 
  } else if (argInfo == ArgType::Brief) {
    for (std::string::const_iterator citer = fullArg.cbegin() + 1u;
    citer != fullArg.cend(); ++citer) {
      result.push_back(arg_t(std::string(1, *citer), ""));
    }
    return result;
  } else if (argInfo == ArgType::Value) {
    const size_t equalIndex = fullArg.find('=');
    return {arg_t(fullArg.substr(2u, equalIndex - 2u),
    fullArg.substr(equalIndex + 1u, fullArg.size() - (equalIndex + 1u)))};
  }
  return {arg_t(fullArg.substr(2u, fullArg.size() - 2u), "")};
}



int argValueToInt(const arg_t& arg, int argIndex, bool& success) {
  int result;
  try {
    result = std::stoi(arg.value);
  } catch(...) {
    CENE_WITH_ARG(NE_3, arg, argIndex);
    success = false;
    return INT_MIN;
  }
  success = true;
  return result;
}
