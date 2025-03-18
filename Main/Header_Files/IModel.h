#pragma once

#include "SQLiteCpp/Database.h"
#include <vector>

namespace ptmk {

class IModel {
public:
  static void CreateTable();
  static void InsertBunch(SQLite::Database &, std::vector<IModel> &);
  static std::vector<IModel> selectAll(SQLite::Database &);

  virtual void insert(SQLite::Database &) = 0;
  virtual void update(SQLite::Database &) = 0;
  virtual void insertUpdate(SQLite::Database &) = 0;

  virtual ~IModel() {}
};

} // namespace ptmk
