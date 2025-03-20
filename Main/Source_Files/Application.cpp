#include <cstdlib>
#include <string>

#include <pqxx/pqxx>

#include "AppSettings.h"
#include "Application.h"
#include "EmployeeGen.h"
#include "EmployeeModel.h"
#include "Logger.h"
#include "Timer.h"

Application::Application(std::string psqlURL) : m_conn(psqlURL) {
  try {
    m_conn = pqxx::connection(psqlURL);
  } catch (std::exception &ex) {
    Logger::GetInstance().Error("Postgresql Exception " +
                                std::string(ex.what()));
  }
}

int Application::run(AppSettings settings) {
  if (!m_conn.is_open()) {
    Logger::GetInstance().Error("Trying to run application when"
                                " the connection is not established.");
    return EXIT_FAILURE;
  }
  try {
    switch (settings.m_mode) {
    case CREATE_TABLE: {
      create_table();
      break;
    }
    case INSERT: {
      insertOne(settings.getEmployee());
      break;
    }
    case SELECT: {
      select();
      break;
    }
    case INSERT_BULK: {
      randomized_bulk_insert();
      break;
    }
    case SELECT_FILTER: {
      select_filter();
      break;
    }
    default:
    case MAX_MODE: {
      Logger::GetInstance().Error("Need to provide mode argument. "
                                  "See --help for more information.");
      return EXIT_FAILURE;
    }
    }
  } catch (std::exception &ex) {
    Logger::GetInstance().Error("Postgresql Exception " +
                                std::string(ex.what()));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
void Application::create_table() { ptmk::EmployeeModel::CreateTable(m_conn); }

void Application::insertOne(ptmk::EmployeeModel employee) {
  employee.insertUpdate(m_conn);
}

void Application::select() {
  Logger::GetInstance().Log("Selecting from database");
  auto data = ptmk::EmployeeModel::select(m_conn);
  for (auto &elem : data)
    Logger::GetInstance().Write(elem);
}

void Application::randomized_bulk_insert() {
  Logger::GetInstance().Log("Start generating pseudorandom employees");
  auto vec = EmployeeGenerator::getEmployeeVector1M();
  Logger::GetInstance().Log("Finished generating pseudorandom employees");

  Logger::GetInstance().Log("Start inserting employees in bulk");
  ptmk::EmployeeModel::BulkInsert(m_conn, std::move(vec));
  Logger::GetInstance().Log("Finished inserting");

  auto vec2 = EmployeeGenerator::getEmployeeVectorMaleF();
  Logger::GetInstance().Log("Got vector of size " +
                            std::to_string(vec2.size()) +
                            " with Males whose surname starting with F");
  ptmk::EmployeeModel::BulkInsert(m_conn, std::move(vec2));
  Logger::GetInstance().Log("Inserted the vector");
}

// TEST UTILITY
static void testFilter(pqxx::connection &conn, std::string filter) {
  Logger::GetInstance().FileLog("Testing clause: " + filter);

  std::vector<ptmk::EmployeeModel> data;
  auto time = Timer::TimeFunction([&data, &conn, &filter]() {
    data = ptmk::EmployeeModel::select(conn, filter);
  });
  Logger::GetInstance().Log("Got vector of size: " +
                            std::to_string(data.size()));

  Logger::GetInstance().FileLog("Retrieving data from database took " + time +
                                " milliseconds");
}

static void createIndex(pqxx::connection &conn, std::string index) {
  pqxx::work tx{conn};

  std::string query = "CREATE INDEX IF NOT EXISTS " + index;

  tx.exec(query);
  tx.commit();
}

static void dropIndex(pqxx::connection &conn, std::string index) {
  pqxx::work tx{conn};

  std::string query = "DROP INDEX IF EXISTS " + index;

  tx.exec(query);
  tx.commit();
}

static void testIndex(pqxx::connection &conn, std::string index) {
  createIndex(conn, "testIndex " + index);
  testFilter(conn, "WHERE fullname LIKE 'F%' AND gender='Male'");
  testFilter(conn, "WHERE left(fullname, 1)='F' AND gender='Male'");
  dropIndex(conn, "testIndex");
}
// TEST UTILITY

void Application::select_filter() {
  Logger::GetInstance().Log("Selecting from database all males "
                            "whose surnames start with F");

  Logger::GetInstance().Log("====== TESTING Table with partitions ======");
  testFilter(m_conn, "WHERE fullname LIKE 'F%' AND gender='Male'");
  testFilter(m_conn, "WHERE left(fullname, 1)='F' AND gender='Male'");

  Logger::GetInstance().Log(
      "====== TESTING Table with B-TREE index of fullname ======");
  testIndex(m_conn, "ON Employee (fullname)");

  Logger::GetInstance().Log(
      "====== TESTING Table with B-TREE index of gender ======");
  testIndex(m_conn, "ON Employee (gender)");

  Logger::GetInstance().Log(
      "====== TESTING Table with B-TREE index of fullname and gender ======");
  testIndex(m_conn, "ON Employee (fullname, gender)");
}
