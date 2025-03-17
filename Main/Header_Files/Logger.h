#pragma once

#include <mutex>
#include <string>

class Logger {
public:
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  static Logger &GetInstance();

  void Write(const std::string &, bool = false);
  void Log(const std::string &);
  void Warning(const std::string &);
  void Error(const std::string &);

  void SetVerbose(bool);

  bool GetVerbose();

private:
  std::mutex m_write_lock;
  bool m_verbose = false;

private:
  Logger() {}

  static std::string GetTime();
};
