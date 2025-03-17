#include <cassert>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "AppMode.h"
#include "AppSettings.h"
#include "EmployeeModel.h"
#include "Logger.h"

// AppSettings public

AppSettings::AppSettings(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string opt = argv[i];

    if (opt[0] == '-' && opt.length() > 2 && opt[1] != '-') {
      Logger::GetInstance().Error(
          "Cannot combine multiple singular flags in one. Ignoring " + opt +
          " flag...");
      continue;
    }

    if (auto j = NoArgs.find(opt); j != NoArgs.end()) {
      j->second();
      continue;
    }

    if (auto k = OneArgs.find(opt); k != OneArgs.end()) {
      if (++i < argc)
        k->second(argv[i]);
      else
        throw std::runtime_error{"Missing param after " + opt};
      continue;
    }

    if (auto k = ThreeArgs.find(opt); k != ThreeArgs.end()) {
      if (i + 3 < argc)
        k->second(argv[i + 1], argv[i + 2], argv[i + 3]);
      else
        throw std::runtime_error{"Missing param after " + opt};
      i += 3;
      continue;
    }

    Logger::GetInstance().Error("Unrecognized command-line option " + opt +
                                ". Ignoring...");
  }
}

ptmk::EmployeeModel AppSettings::getEmployee() {
  if (m_employee)
    return m_employee.value();
  Logger::GetInstance().Error("Employee not provided");
  throw std::runtime_error("Employee not provided");
}

// AppSettings private

void AppSettings::setHelpToTrue() { m_help = true; }
void AppSettings::setVerboseToTrue() { Logger::GetInstance().SetVerbose(true); }
void AppSettings::setOutfile(std::string fileName) {
  m_outfile = fileName.substr(0, fileName.find("."));
}
void AppSettings::setMode(AppMode mode) {
  if (m_mode != MAX_MODE) {
    Logger::GetInstance().Error("Provided multiple mode arguments. "
                                "Ignoring everything except the first one.");
    return;
  }
  m_mode = mode;
}
void AppSettings::createEmployee(std::string name, std::string birthDate,
                                 std::string gender) {
  m_employee = ptmk::EmployeeModel(name, birthDate, gender);
}
