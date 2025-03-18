#include <chrono>
#include <ctime>
#include <iostream>
#include <ostream>

#include "Logger.h"

// Logger public
int a = 0;

Logger &Logger::GetInstance() {
  static Logger logger;
  return logger;
}

#include <iostream>
#include <chrono>
// #include <iomanip>
// #include <sstream>


// !!!
// std::string Logger::GetTime() {
//     const auto now = std::chrono::system_clock::now();
//
//     const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
//
//     std::tm tm = *std::localtime(&t_c);
//
//     const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//                         now.time_since_epoch()) % 1000;
//
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%a %b %d %H:%M:%S") << "." << std::setfill('0') 
//         << std::setw(3) << ms.count();
//
//     return oss.str();
// }

std::string Logger::GetTime() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
  std::string timeStr = std::ctime(&t_c); // Get the time string with newline
  if (!timeStr.empty() && timeStr.back() == '\n') {
    timeStr.pop_back(); // Remove the newline character
  }
  return timeStr;
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
    std::cout << "[LOG  " << GetTime() << "] " << message << std::endl;
  }
}
void Logger::Warning(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cerr << "[WARN " << GetTime() << "] " << message << std::endl;
  }
}

void Logger::Error(const std::string &message) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cerr << "[ERR  " << GetTime() << "] " << message << std::endl;
}

void Logger::SetVerbose(bool verbose) { m_verbose = verbose; }

bool Logger::GetVerbose() { return m_verbose; }
