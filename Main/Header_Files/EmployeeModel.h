#pragma once

#include <chrono>
#include <ostream>
#include <string>
#include <vector>

#include "SQLiteCpp/Database.h"

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
  EmployeeModel();
  EmployeeModel(std::string, std::string, std::string);
  EmployeeModel(std::string, std::chrono::year_month_day, Gender);
  ~EmployeeModel();

  static void CreateTable(SQLite::Database &);
  static void InsertBunch(SQLite::Database &, std::vector<EmployeeModel> &&);
  static std::vector<EmployeeModel> select(SQLite::Database &,
                                           std::string options = "");

  void insert(SQLite::Database &) override;
  void update(SQLite::Database &) override;
  void insertUpdate(SQLite::Database &) override;

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

  inline static size_t MAX_BUNCH_SIZE = 1000000;
};

} // namespace ptmk
