#pragma once

#include "AppSettings.h"

class Application {
public:
  Application(std::string);

  int run(AppSettings);

private:
  pqxx::connection m_conn;

private:
  void create_table();
  void insertOne(ptmk::EmployeeModel);
  void select();
  void randomized_bulk_insert();
  void select_filter();
};
