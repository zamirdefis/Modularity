#include <vector>

#include "macros.hpp"

[[nodiscard]] std::vector<ull_t> getCPUTimes(void) noexcept;

[[nodiscard]] inline ull_t getTotalTime(const std::vector<ull_t>& times) noexcept;

[[nodiscard]] float getCPUUsage(void) noexcept;
