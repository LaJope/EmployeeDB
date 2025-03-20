#include <chrono>
#include <functional>
#include <string>

class Timer {
public:
  using tp_sc = std::chrono::time_point<std::chrono::system_clock>;

  static tp_sc GetTime();

  static std::string GetTimeString();
  static long GetDifference(const tp_sc &);

  static std::string TimeFunction(std::function<void(void)>);

private:
  Timer() = delete;
};
