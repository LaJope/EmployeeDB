#include <cstdlib>

#include "SQLiteCpp/Database.h"

#include "AppSettings.h"
#include "Application.h"
#include "EmployeeGen.h"
#include "EmployeeModel.h"
#include "Logger.h"

Application::Application(int argc, char *argv[]) : m_settings(argc, argv) {}

int Application::run() {
  try {
    SQLite::Database db = SQLite::Database(
        "test.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    Logger::GetInstance().Log("Opened sqlite database file: " +
                              db.getFilename());

    if (m_settings.m_query)
      db.exec(m_settings.m_query.value());

    switch (m_settings.m_mode) {
    case CREATE_TABLE: {
      create_table(db);
      break;
    }
    case INSERT: {
      insertOne(db);
      break;
    }
    case SELECT: {
      select(db);
      break;
    }
    case INSERT_BUNCH: {
      randomized_insert_bunch(db);
      break;
    }
    case SELECT_FILTER: {
      select_filter(db);
      break;
    }
    default:
    case MAX_MODE: {
      Logger::GetInstance().Error("Need to provide mode argument. "
                                  "See --help for more information.");
      break;
    }
    }
  } catch (std::exception &ex) {
    Logger::GetInstance().Error("SQLite Exception " + std::string(ex.what()));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
void Application::create_table(SQLite::Database &db) {
  ptmk::EmployeeModel::CreateTable(db);
}
void Application::insertOne(SQLite::Database &db) {
  m_settings.getEmployee().insertUpdate(db);
}
void Application::select(SQLite::Database &db) {
  Logger::GetInstance().Log("Selecting from database");
  auto data = ptmk::EmployeeModel::select(db);
  for (auto &elem : data)
    Logger::GetInstance().Write(elem);
}
void Application::randomized_insert_bunch(SQLite::Database &db) {
  Logger::GetInstance().Log("Start generating pseudorandom employees");
  auto vec = EmployeeGenerator::getEmployeeVector1M();
  Logger::GetInstance().Log("Finished generating pseudorandom employees");

  Logger::GetInstance().Log("Start inserting each employee one by one");
  ptmk::EmployeeModel::InsertBunch(db, std::move(vec));
  Logger::GetInstance().Log("Finished inserting");
}
void Application::select_filter(SQLite::Database &db) {
  std::string filter = "WHERE \"fullname\" LIKE \"F%\" AND \"gender\"=\"Male\"";
  Logger::GetInstance().Log("Selecting from database all males "
                            "whose surnames start with F");
  auto data = ptmk::EmployeeModel::select(db, filter);
  Logger::GetInstance().Log("Got vector from database");
  Logger::GetInstance().Log("Vector size: " + std::to_string(data.size()));
  // for (auto &elem : data)
  //   Logger::GetInstance().Write("Employee = " + std::string(elem));
}
