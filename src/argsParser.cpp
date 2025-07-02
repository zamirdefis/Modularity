#include "argsParser.hpp"
#include "notifications.hpp"

#include <climits>

[[nodiscard]] ArgType getArgType(std::string_view fullArg) {
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

[[nodiscard]] std::vector<std::pair<std::string, std::string>> parseArg(const std::string& fullArg) {
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



int argValueToInt(const std::pair<std::string, std::string>& arg, int argIndex, bool& success) {
  int result;
  try {
    result = std::stoi(arg.second);
  } catch(...) {
    // co::error(static_cast<std::string>(NE_3) + " [" +
    //           std::to_string(argIndex) + "] : " + arg.first + "=" + arg.second);
    CENE_WITH_ARG(NE_3, arg, argIndex);
    success = false;
    return INT_MIN;
  }
  success = true;
  return result;
}
