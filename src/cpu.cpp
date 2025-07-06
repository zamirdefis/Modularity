#include "cpu.hpp"

#include <fstream>
#include <chrono>
#include <thread>

[[nodiscard]] std::vector<ull_t> getCPUTimes(void) noexcept {
  std::ifstream procStat("/proc/stat");
  if (!procStat.is_open()) { return {}; }
  procStat.ignore(3);
  std::vector<ull_t> times;
  for (ull_t time; procStat >> time; times.push_back(time));
  return times;
}

[[nodiscard]] inline ull_t getTotalTime(const std::vector<ull_t>& times) noexcept {
  if (times.size() < 4u) { return static_cast<ull_t>(NULL); }
  return times[static_cast<size_t>(NULL)] + times[1u] + times[2u] + times[3u]; // user + nice + system + idle
}

[[nodiscard]] float getCPUUsage(void) noexcept {
  const std::vector<ull_t> prevCPUTimes = getCPUTimes();
  const ull_t prevTotal = getTotalTime(prevCPUTimes);
  if (prevTotal == static_cast<ull_t>(NULL)) { return -1.f; }
  const ull_t prevIdle = prevCPUTimes[3u];

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  const decltype(prevCPUTimes) currTimes = getCPUTimes();
  const ull_t currTotal = getTotalTime(currTimes);
  if (currTotal == static_cast<ull_t>(NULL)) { return -1.f; }
  const ull_t currIdle = currTimes[3u];

  if (currTotal < prevTotal || currIdle < prevIdle) {
    return -1.f;
  }

  const double totalDiff = static_cast<double>(currTotal - prevTotal);
  const double idleDiff = static_cast<double>(currIdle - prevIdle);
  if (totalDiff == static_cast<double>(NULL)) {
    return static_cast<float>(NULL);
  }
  return static_cast<float>(100.f * (totalDiff - idleDiff) / totalDiff);
}
