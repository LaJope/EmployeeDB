#include <chrono>
#include <cstddef>
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
  const std::string create_query =
      "CREATE TABLE Employee ("
      "ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
      "fullname VARCHAR(100) NOT NULL, "
      "birthDate DATE NOT NULL "
      "CHECK(birthDate IS strftime('%Y-%m-%d', birthDate)), "
      "gender TEXT CHECK(gender IN (\"Male\", \"Female\")) NOT NULL)";
  db.exec(create_query);
  Logger::GetInstance().Log("Created Employee table");
}

void EmployeeModel::InsertBunch(SQLite::Database &db,
                                std::vector<EmployeeModel> &&data) {
  static size_t FMale = 0;
  size_t dataSize = data.size();
  std::string base_query =
      "INSERT INTO Employee (fullname, birthDate, gender) VALUES";
  std::string curr_query = base_query;
  Logger::GetInstance().Log("Got " + std::to_string(dataSize) +
                            " Employees to insert into database");
  for (size_t i = 0; i < dataSize; i++) {
    if (data[i].getName()[0] == 'F' && data[i].getGenderString() == "Male")
      FMale++;
    curr_query += " " + std::string(data[i]);
    if ((i + 1) % MAX_BUNCH_SIZE == 0 || (i == dataSize - 1)) {
      SQLite::Statement stmt(db, curr_query);
      stmt.executeStep();
      Logger::GetInstance().Log("Sent " +
                                std::to_string(i % MAX_BUNCH_SIZE + 1) +
                                " Employees to database");
      curr_query = base_query;
    } else
      curr_query += ",";
  }
  Logger::GetInstance().Log("Amount of Males with F: " + std::to_string(FMale));
}

std::vector<EmployeeModel> EmployeeModel::select(SQLite::Database &db,
                                                 std::string options) {
  std::string select = "SELECT * FROM Employee " + options;

  std::vector<EmployeeModel> result;

  SQLite::Statement query(db, select);
  while (query.executeStep()) {
    result.emplace_back(std::string(query.getColumn(1)),
                        std::string(query.getColumn(2)),
                        std::string(query.getColumn(3)));
  }

  return result;
}

void EmployeeModel::insert(SQLite::Database &db) {
  std::string query =
      "INSERT INTO Employee (fullname, birthDate, gender) VALUES (?, ?, ?)";

  SQLite::Statement stmt(db, query);
  stmt.bind(1, m_fullname);
  stmt.bind(2, getBirthDateString());
  stmt.bind(3, getGenderString());
  // Logger::GetInstance().Log(stmt.getExpandedSQL());
  stmt.executeStep();
  query = "SELECT last_insert_rowid()";
  stmt = {db, query};
  stmt.executeStep();
  m_id = stmt.getColumn(0);
  Logger::GetInstance().Log("New Employee is at index: " +
                            std::to_string(m_id));
}

void EmployeeModel::update(SQLite::Database &db) {
  std::string query =
      "UPDATE Employee SET fullname=?, birthDate=?, gender=? WHERE ID=?";
  SQLite::Statement stmt(db, query);
  stmt.bind(1, m_fullname);
  stmt.bind(2, getBirthDateString());
  stmt.bind(3, getGenderString());
  stmt.bind(4, m_id);
  Logger::GetInstance().Log(stmt.getExpandedSQL());
  stmt.executeStep();
}

void EmployeeModel::insertUpdate(SQLite::Database &db) {
  if (!isValid()) {
    Logger::GetInstance().Error(
        "Trying to insert/update invalid Employee object (all fields must be "
        "initialized through constructor)");
    return;
  }
  if (m_id == -1)
    insert(db);
  else
    update(db);
}

int64_t EmployeeModel::getID() { return m_id; }
std::string EmployeeModel::getName() { return m_fullname; }
std::string EmployeeModel::getBirthDateString() {
  std::ostringstream str;
  str << m_birthDate;
  return str.str();
}
std::string EmployeeModel::getGenderString() {
  return (m_gender == MALE ? "Male" : "Female");
}

bool EmployeeModel::isValid() { return m_id != -2; }

std::ostream &EmployeeModel::operator<<(std::ostream &out) {
  if (!isValid()) {
    Logger::GetInstance().Error(
        "Trying to print invalid Employee object (all fields must be "
        "initialized through constructor)");
    return out;
  }
  out << "Employee = { " << m_fullname << " ; " << m_birthDate << " ; "
      << getGenderString() << " }";
  return out;
}

EmployeeModel::operator std::string() {
  return "( '" + m_fullname + "' , '" + getBirthDateString() + "' , '" +
         getGenderString() + "' )";
}

} // namespace ptmk
