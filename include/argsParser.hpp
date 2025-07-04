#pragma once

#define CENE_WITH_ARG(NE, ARG, ARG_INDEX) co::error(static_cast<std::string>(NE) + " [" +\
              std::to_string(ARG_INDEX) + "] : " + ARG.main + "=" + ARG.value)

#include <cstdint>
#include <vector>
#include <string>

class arg_t final {
public:
  std::string main;
  std::string value;
  arg_t(void);
  arg_t(std::string_view);
  arg_t(std::string_view, std::string_view);
};

enum class ArgType : uint8_t {
  Invalid,
  Brief,
  Single,
  Value,
};

[[nodiscard]] ArgType getArgType(std::string_view fullArg);

[[nodiscard]] std::vector<arg_t> parseArg(const std::string& fullArg);

int argValueToInt(const arg_t& arg, int argIndex, bool& success);
