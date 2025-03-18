#include <chrono>
#include <string>

class Timer {
public:
  using tp_sc = std::chrono::time_point<std::chrono::system_clock>;

  static tp_sc GetTime();

  static std::string GetTimeString();
  static std::string GetDifferenceString(const tp_sc &);
};
