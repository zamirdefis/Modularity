#pragma once

#include <string>
#include <unordered_map>

#include "macros.hpp"

struct mem_t final {
  ull_t MemTotal;
  ull_t MemFree;
  mem_t(void);
};

ull_t svtoull_s(const std::string_view& str, bool& success);

[[nodiscard]] std::unordered_map<std::string, ull_t>* getMemInfoMap(void);

bool getRAM(mem_t& res);
