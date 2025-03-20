#pragma once

#include <chrono>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <pqxx/pqxx>

#include "IModel.h"

namespace ptmk {

class EmployeeModel : IModel {
public:
  enum Gender { MALE, FEMALE };
  enum Sort {
    DESC,
    EMPLOYEE_FULLNAME,
    EMPLOYEE_BIRTHDATE,
    EMPLOYEE_GENDER,
  };

public:
  EmployeeModel(int64_t, std::string, std::string, std::string);
  EmployeeModel(std::string, std::string, std::string);
  EmployeeModel(std::string, std::chrono::year_month_day, Gender);
  ~EmployeeModel();

  static void CreateTable(pqxx::connection &);
  static void BulkInsert(pqxx::connection &, std::vector<EmployeeModel> &&);
  static std::vector<EmployeeModel> select(pqxx::connection &,
                                           std::string options = "");

  void insert(pqxx::connection &) override;
  void update(pqxx::connection &) override;
  void insertUpdate(pqxx::connection &) override;

  bool isValid();
  int64_t getID();
  std::string getName();
  std::string getBirthDateString();
  std::string getGenderString();

  std::ostream &operator<<(std::ostream &);

  operator std::string();

private:
  int64_t m_id = -2;
  std::string m_fullname;
  std::chrono::year_month_day m_birthDate;
  Gender m_gender;

  inline static size_t MAX_PARAM_SIZE = 65535;
};

} // namespace ptmk
