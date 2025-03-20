#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>
#include <string>

#include "Timer.h"

Timer::tp_sc Timer::GetTime() {
  using namespace std::chrono;

  const auto now = high_resolution_clock::now();
  return now;
}

std::string Timer::GetTimeString() {
  using namespace std::chrono;

  const auto now = system_clock::now();
  const std::time_t time = system_clock::to_time_t(now);

  std::tm tm = *std::localtime(&time);

  const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream oss;
  oss << std::put_time(&tm, "%a %b %d %H:%M:%S") << "." << std::setfill('0')
      << std::setw(3) << ms.count();

  return oss.str();
}

long Timer::GetDifference(const tp_sc &time) {
  using namespace std::chrono;

  const auto duration =
      duration_cast<milliseconds>(high_resolution_clock::now() - time);

  return duration.count();
}

std::string Timer::TimeFunction(std::function<void(void)> func) {
  auto start = GetTime();
  func();
  auto funcTime = GetDifference(start);
  return std::to_string(funcTime);
}
