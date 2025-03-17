#include "EmployeeModel.h"
#include "SQLiteCpp/Database.h"

#include "AppSettings.h"
#include "Application.h"
#include "Logger.h"
#include <cstdlib>
#include <vector>

Application::Application(int argc, char *argv[]) : m_settings(argc, argv) {}

int Application::run() {
  try {
    SQLite::Database db = SQLite::Database(
        "test.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    Logger::GetInstance().Log("Opened sqlite database file: " +
                              db.getFilename());

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
      select_filter(db, "");
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
  m_settings.getEmployee().insertInto(db);
}
void Application::select(SQLite::Database &db) {
  auto data = ptmk::EmployeeModel::selectAll(db);
  for (auto &elem : data)
    Logger::GetInstance().Write(elem);
}
void Application::randomized_insert_bunch(SQLite::Database &db) {
  std::vector<ptmk::EmployeeModel> a;
  ptmk::EmployeeModel::InsertBunch(db, a);
}
void Application::select_filter(SQLite::Database &db, std::string filter) {
  ptmk::EmployeeModel::selectFilter(db, filter, 0);
}
