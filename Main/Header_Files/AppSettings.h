#pragma once

#include <cstdlib>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "EmployeeModel.h"
#include "AppMode.h"

class AppSettings {
public:
  bool m_help = false;

  AppMode m_mode = MAX_MODE;

public:
  AppSettings(int argc, char *argv[]);

  ptmk::EmployeeModel getEmployee();

private:
  std::optional<ptmk::EmployeeModel> m_employee;

private:
  using NoArgHandle = std::function<void()>;
  using OneArgHandle = std::function<void(std::string)>;
  using ThreeArgHandle =
      std::function<void(std::string, std::string, std::string)>;

  const std::unordered_map<std::string, NoArgHandle> NoArgs{
      {"-h", [this]() { setHelpToTrue(); }},
      {"--help", [this]() { setHelpToTrue(); }},

      {"-v", [this]() { setVerboseToTrue(); }},
      {"--verbose", [this]() { setVerboseToTrue(); }},

      {"1", [this]() { setMode(CREATE_TABLE); }},
      {"3", [this]() { setMode(SELECT); }},
      {"4", [this]() { setMode(INSERT_BULK); }},
      {"5", [this]() { setMode(SELECT_FILTER); }},
  };

  const std::unordered_map<std::string, OneArgHandle> OneArgs{
      {"-o", [this](std::string v) { setLogfile(v); }},
      {"--outfile", [this](std::string v) { setLogfile(v); }},
  };

  const std::unordered_map<std::string, ThreeArgHandle> ThreeArgs{
      {"2",
       [this](std::string name, std::string birthDate, std::string gender) {
         setMode(INSERT);
         createEmployee(name, birthDate, gender);
       }},
  };

private:
  void setHelpToTrue();
  void setVerboseToTrue();
  void setLogfile(std::string);
  void setMode(AppMode);
  void createEmployee(std::string, std::string, std::string);
};
