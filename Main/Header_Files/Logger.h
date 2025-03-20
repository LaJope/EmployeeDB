#pragma once

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

  void FileLog(const std::string &);

  void SetVerbose(bool);
  void SetLogfile(std::string);

private:
  bool m_verbose = false;

  std::string m_logfile = "ptmk";

private:
  Logger() {}
};
