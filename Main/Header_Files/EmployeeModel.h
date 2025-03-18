#pragma once

#include <chrono>
#include <ostream>
#include <string>
#include <vector>

#include "SQLiteCpp/Database.h"

#include "IModel.h"

namespace ptmk {

enum EmployeeSort {
  DESC,
  EMPLOYEE_FULLNAME,
  EMPLOYEE_BIRTHDATE,
  EMPLOYEE_GENDER,
};

class EmployeeModel : IModel {
public:
  enum Gender { MALE, FEMALE };

public:
  EmployeeModel();
  EmployeeModel(std::string, std::string, std::string);
  EmployeeModel(std::string, std::chrono::year_month_day, Gender);
  ~EmployeeModel();

  static void CreateTable(SQLite::Database &);
  static void InsertBunch(SQLite::Database &, std::vector<EmployeeModel> &);
  static std::vector<EmployeeModel> selectAll(SQLite::Database &, uint8_t = 0);
  static std::vector<EmployeeModel> selectFilter(SQLite::Database &,
                                                 std::string, uint8_t);

  void insertInto(SQLite::Database &) override;

  int64_t getID();
  std::string getName();
  std::string getBirthDateString();
  std::string getGenderString();

  bool isValid();

  std::ostream &operator<<(std::ostream &);

  operator std::string();

private:
  int64_t m_id = -2;
  std::string m_fullname;
  std::chrono::year_month_day m_birthDate;
  Gender m_gender;

private:
};

} // namespace ptmk
