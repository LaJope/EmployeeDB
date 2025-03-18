#pragma once

#include "AppSettings.h"

class Application {
public:
  Application(int argc, char *argv[]);

  int run();

private:
  AppSettings m_settings;
  // SQLite::Database m_db;

private:
  void create_table(SQLite::Database &);
  void insertOne(SQLite::Database &);
  void select(SQLite::Database &);
  void randomized_insert_bunch(SQLite::Database &);
  void select_filter(SQLite::Database &);
};
