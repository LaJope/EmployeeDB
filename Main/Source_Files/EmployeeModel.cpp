#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "SQLiteCpp/Statement.h"

#include "EmployeeModel.h"
#include "Logger.h"

namespace ptmk {

// EmployeeModel public

EmployeeModel::EmployeeModel() {}
EmployeeModel::EmployeeModel(std::string name, std::string birthDate,
                             std::string gender) {
  m_fullname = name;
  std::istringstream dateParse(birthDate);
  dateParse >> std::chrono::parse("%Y-%m-%d", m_birthDate);
  if (dateParse.fail()) {
    Logger::GetInstance().Error("Failed to parse birth date. "
                                "Object invalid. Date provided: " +
                                birthDate);
    return;
  }
  if (gender != "Male" && gender != "Female") {
    Logger::GetInstance().Error("Failed to parse gender. "
                                "Object invalid. Gender provided: " +
                                gender);
    return;
  }
  m_gender = (gender == "Male" ? MALE : FEMALE);
  m_id = -1;
}
EmployeeModel::EmployeeModel(std::string name,
                             std::chrono::year_month_day birthDate,
                             Gender gender)
    : m_id(-1), m_fullname(name), m_birthDate(birthDate), m_gender(gender) {}
EmployeeModel::~EmployeeModel() {}

void EmployeeModel::CreateTable(SQLite::Database &db) {
  db.exec("CREATE TABLE Employee ("
          "ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
          "fullname VARCHAR(100) NOT NULL, "
          "birthDate DATE NOT NULL "
          "CHECK(birthDate IS strftime('%Y-%m-%d', birthDate)), "
          "gender TEXT CHECK(gender IN (\"Male\", \"Female\")) NOT NULL)");
  Logger::GetInstance().Log("Created Employee table");
}
void EmployeeModel::InsertBunch(SQLite::Database &,
                                std::vector<EmployeeModel> &) {}
std::vector<EmployeeModel> EmployeeModel::selectAll(SQLite::Database &db,
                                                    uint8_t sort) {
  std::string select = "SELECT * FROM Employee";
  if (sort & EMPLOYEE_FULLNAME) {
    select += " ORDER BY fullname";
    select += +((sort & DESC) ? " DESC" : " ASC");
  }
  if (sort & EMPLOYEE_BIRTHDATE) {
    select += " ORDER BY birthDate";
    select += +((sort & DESC) ? " DESC" : " ASC");
  }
  if (sort & EMPLOYEE_GENDER) {
    select += " ORDER BY gender";
    select += +((sort & DESC) ? " DESC" : " ASC");
  }

  std::vector<EmployeeModel> result;

  SQLite::Statement query(db, select);
  while (query.executeStep()) {
    result.emplace_back(std::string(query.getColumn(1)),
                        std::string(query.getColumn(2)),
                        std::string(query.getColumn(3)));
  }

  return result;
}
std::vector<EmployeeModel> EmployeeModel::selectFilter(SQLite::Database &,
                                                       std::string, uint8_t) {
  return {};
}

void EmployeeModel::insertInto(SQLite::Database &db) {
  std::string query;
  if (m_id == -1)
    query = "INSERT INTO Employee (fullname, birthDate, gender) "
            "VALUES (?, ?, ?)";
  else
    query = "UPDATE Employee SET fullname=?, birthDate=?, gender=? WHERE ID=?";
  SQLite::Statement stmt(db, query);
  stmt.bind(1, m_fullname);
  stmt.bind(2, getBirthDateString());
  stmt.bind(3, getGenderString());
  if (m_id != -1)
    stmt.bind(4, m_id);
  Logger::GetInstance().Log(query + "\n\tDATA: " + m_fullname + " " +
                            getBirthDateString() + " " + getGenderString());
  stmt.executeStep();
}

std::ostream &EmployeeModel::operator<<(std::ostream &out) {
  if (!isValid()) {
    Logger::GetInstance().Error(
        "Trying to print invalid Employee object (all fields must be "
        "initialized through constructors)");
    return out;
  }
  out << "Employee = { " << m_fullname << " ; " << m_birthDate << " ; "
      << getGenderString() << " }";
  return out;
}

EmployeeModel::operator std::string() {
  return "Employee = { " + m_fullname + " ; " + getBirthDateString() + " ; " +
         getGenderString() + " }";
}

} // namespace ptmk
