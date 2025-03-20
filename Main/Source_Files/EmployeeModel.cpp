#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <pqxx/pqxx>

#include "EmployeeModel.h"
#include "Logger.h"

namespace ptmk {

// EmployeeModel public

EmployeeModel::EmployeeModel(int64_t id, std::string name,
                             std::string birthDate, std::string gender)
    : ptmk::EmployeeModel(name, birthDate, gender) {
  m_id = id;
}
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

void EmployeeModel::CreateTable(pqxx::connection &conn) {
  pqxx::work tx{conn};
  /*
  * std::string query = "CREATE TYPE sex AS ENUM('Male', 'Female');"
  *                     "CREATE TABLE Employee ("
  *                     "ID SERIAL PRIMARY KEY, "
  *                     "fullname VARCHAR(100) NOT NULL, "
  *                     "birthDate DATE NOT NULL, "
  *                     "gender sex NOT NULL); ";
  tx.exec(query);
  */

  std::string query = "CREATE TYPE sex AS ENUM('Male', 'Female');"
                      "CREATE TABLE Employee ("
                      "ID SERIAL, "
                      "fullname VARCHAR(100) NOT NULL, "
                      "birthDate DATE NOT NULL, "
                      "gender sex NOT NULL) "
                      "PARTITION BY LIST (left(fullname, 1));";
  tx.exec(query);

  query = "CREATE OR REPLACE FUNCTION create_emp_partitions() "
          "RETURNS void AS $$ "
          "DECLARE "
          "letter CHAR; "
          "BEGIN "
          "FOR letter IN "
          "SELECT chr(generate_series(ascii('A'), ascii('Z'))) LOOP "
          "EXECUTE format('CREATE TABLE EmployeeSt%s PARTITION OF Employee "
          "FOR VALUES IN (%L);', letter, letter); "
          "END LOOP; "
          "END; "
          "$$ LANGUAGE plpgsql;";
  tx.exec(query);

  query = "SELECT create_emp_partitions();";
  tx.exec(query);

  Logger::GetInstance().Log("Created partitioned Employee table");
  tx.commit();
}

void EmployeeModel::BulkInsert(pqxx::connection &conn,
                               std::vector<EmployeeModel> &&data) {
  pqxx::work tx{conn};

  size_t dataSize = data.size();
  Logger::GetInstance().Log("Got " + std::to_string(dataSize) +
                            " Employees to insert into database");

  std::string baseQuery =
      "INSERT INTO Employee (fullname, birthDate, gender) VALUES ";

  std::string currQuery = baseQuery;
  size_t valuesCount = 0;
  pqxx::params params{};

  for (size_t i = 0; i < dataSize; i++) {
    auto employee = data[i];
    currQuery += std::string(valuesCount > 0 ? ", " : "") + "($" +
                 std::to_string(valuesCount * 3 + 1) + ", $" +
                 std::to_string(valuesCount * 3 + 2) + ", $" +
                 std::to_string(valuesCount * 3 + 3) + ")";
    valuesCount++;

    params.append(employee.getName());
    params.append(employee.getBirthDateString());
    params.append(employee.getGenderString());

    if (params.size() + 3 >= MAX_PARAM_SIZE || (i == dataSize - 1)) {
      pqxx::result result = tx.exec(currQuery, params);

      if (static_cast<size_t>(result.affected_rows()) != valuesCount)
        Logger::GetInstance().Error("Could not send full bulk. Only sent " +
                                    std::to_string(result.affected_rows()) +
                                    " out of " + std::to_string(valuesCount));
      // else
      // Logger::GetInstance().Log("Sent " + std::to_string(valuesCount) +
      //                           " Employees to database");

      valuesCount = 0;
      params = pqxx::params();
      currQuery = baseQuery;
    }
  }
  tx.commit();
}

std::vector<EmployeeModel> EmployeeModel::select(pqxx::connection &conn,
                                                 std::string options) {
  pqxx::work tx{conn};
  std::string query = "SELECT * FROM Employee " + options;

  std::vector<EmployeeModel> employees;

  pqxx::result result = tx.exec(query);

  for (auto row : result) {
    int64_t id;
    row["ID"].to(id);
    employees.emplace_back(id, row["fullname"].c_str(),
                           row["birthDate"].c_str(), row["gender"].c_str());
  }

  return employees;
}

void EmployeeModel::insert(pqxx::connection &conn) {
  if (!isValid()) {
    Logger::GetInstance().Error(
        "Trying to insert/update invalid Employee object (all fields must be "
        "initialized through constructor)");
    return;
  }
  pqxx::work tx{conn};
  std::string query = "INSERT INTO Employee (fullname, birthDate, gender) "
                      "VALUES ($1, $2, $3) RETURNING ID";

  pqxx::params params{m_fullname, getBirthDateString(), getGenderString()};
  pqxx::result result = tx.exec(query, params).expect_rows(1);

  tx.commit();

  Logger::GetInstance().Log(result.query());
  if (result[0]["ID"].to(m_id))
    Logger::GetInstance().Log("New employee with id: " + std::to_string(m_id));
}

void EmployeeModel::update(pqxx::connection &conn) {
  pqxx::work tx{conn};
  std::string query =
      "UPDATE Employee SET fullname=$1, birthDate=$2, gender=$3 WHERE ID=$4";
  pqxx::params params{m_fullname, getBirthDateString(), getGenderString(),
                      m_id};
  pqxx::result result = tx.exec(query, params);

  Logger::GetInstance().Log(result.query());

  tx.commit();
}

void EmployeeModel::insertUpdate(pqxx::connection &conn) {
  if (!isValid()) {
    Logger::GetInstance().Error(
        "Trying to insert/update invalid Employee object (all fields must be "
        "initialized through constructor)");
    return;
  }
  if (m_id == -1)
    insert(conn);
  else
    update(conn);
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
