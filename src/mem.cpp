#include "mem.hpp"

#include <fstream>
#include <climits>
#include <cstdint>

#define MEMINFO_FILE_PATH "/proc/meminfo"

mem_t::mem_t() {
  this->MemFree = static_cast<ull_t>(NULL);
  this->MemTotal = static_cast<ull_t>(NULL);
}

ull_t svtoull_s(const std::string_view& str, bool& success) {
  ull_t result;
  try {
    result = std::stoull(str.cbegin());
  } catch (...) {
    success = false;
    return ULLONG_MAX;
  }
  success = true;
  return result;
} 

[[nodiscard]] std::unordered_map<std::string, ull_t>* getMemInfoMap(void) {
   std::ifstream meminfof(MEMINFO_FILE_PATH);
  if (!meminfof.is_open()) { return nullptr; }
  std::string line = "";
  std::unordered_map<std::string, ull_t>* memInfoUMapPtr = new std::unordered_map<std::string, ull_t>;
  while (std::getline(meminfof, line)) {
    std::string_view keySV(line);
    std::string_view valueSV(line);
    size_t keyEndIndex = keySV.find(':');
    if (keyEndIndex == SIZE_MAX) {
      continue;
    }
    keySV.remove_suffix(keySV.size() - keyEndIndex);
    valueSV.remove_suffix(3u);
    size_t valueBegin = valueSV.find_first_of(' ');
    valueSV.remove_prefix(valueBegin);
    bool success;
    ull_t value = svtoull_s(valueSV, success);
    if (!success) { continue; }
    (*memInfoUMapPtr)[std::string(keySV)] = value;
  }
  return memInfoUMapPtr;
}

bool getRAM(mem_t& res) {
  std::unordered_map<std::string, ull_t>* memInfoUMapPtr = getMemInfoMap();
  if (memInfoUMapPtr->find("MemTotal") == memInfoUMapPtr->end() || memInfoUMapPtr->find("MemFree") == memInfoUMapPtr->end()) {
    delete memInfoUMapPtr;
    return false;
  }
  res.MemFree = (*memInfoUMapPtr)["MemAvailable"];
  res.MemTotal = (*memInfoUMapPtr)["MemTotal"];
  delete memInfoUMapPtr;
  return true;
}
