#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>

#include "Logger.h"
#include "Timer.h"

// Logger public

Logger &Logger::GetInstance() {
  static Logger logger;
  return logger;
}

void Logger::Write(const std::string &message, bool noNewline) {
  std::cout << message;
  if (noNewline)
    std::cout << std::flush;
  else
    std::cout << std::endl;
}
void Logger::Log(const std::string &message) {
  if (m_verbose)
    std::cout << "[LOG  " << Timer::GetTimeString() << "] " << message
              << std::endl;
}
void Logger::Warning(const std::string &message) {
  if (m_verbose)
    std::cerr << "[WARN " << Timer::GetTimeString() << "] " << message
              << std::endl;
}

void Logger::Error(const std::string &message) {
  std::cerr << "[ERR  " << Timer::GetTimeString() << "] " << message
            << std::endl;
}

void Logger::FileLog(const std::string &message) {
  std::ofstream logFile(m_logfile + ".log", std::ios_base::app);
  logFile << "[LOG  " << Timer::GetTimeString() << "] " << message << "\n";
  Log(message);
}

void Logger::SetVerbose(bool verbose) { m_verbose = verbose; }
void Logger::SetLogfile(std::string logfile) { m_logfile = logfile; }
