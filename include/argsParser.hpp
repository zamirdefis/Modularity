#pragma once

#define CENE_WITH_ARG(NE, ARG, ARG_INDEX) co::error(static_cast<std::string>(NE) + " [" +\
              std::to_string(ARG_INDEX) + "] : " + ARG.first + "=" + ARG.second)

#include <cstdint>
#include <vector>
#include <string>

enum class ArgType : uint8_t {
  Invalid,
  Brief,
  Single,
  Value,
};

[[nodiscard]] ArgType getArgType(std::string_view fullArg);

[[nodiscard]] std::vector<std::pair<std::string, std::string>> parseArg(const std::string& fullArg);

int argValueToInt(const std::pair<std::string, std::string>& arg, int argIndex, bool& success);
