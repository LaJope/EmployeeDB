#include <ctime>
#include <iostream>
#include <ostream>

#include "Logger.h"
#include "Timer.h"

// Logger public
int a = 0;

Logger &Logger::GetInstance() {
  static Logger logger;
  return logger;
}

void Logger::Write(const std::string &message, bool noNewline) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cout << message;
  if (noNewline)
    std::cout << std::flush;
  else
    std::cout << std::endl;
}
void Logger::Log(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cout << "[LOG  " << Timer::GetTimeString() << "] " << message << std::endl;
  }
}
void Logger::Warning(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cerr << "[WARN " << Timer::GetTimeString() << "] " << message << std::endl;
  }
}

void Logger::Error(const std::string &message) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cerr << "[ERR  " << Timer::GetTimeString() << "] " << message << std::endl;
}

void Logger::SetVerbose(bool verbose) { m_verbose = verbose; }

bool Logger::GetVerbose() { return m_verbose; }
