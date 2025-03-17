#pragma once

#include "SQLiteCpp/Database.h"
#include <vector>

namespace ptmk {

class IModel {
public:
  static void CreateTable();
  static void InsertBunch(SQLite::Database &, std::vector<IModel> &);
  static std::vector<IModel> selectAll(SQLite::Database &);

  virtual void insertInto(SQLite::Database &) = 0;

  virtual ~IModel() {}
};

} // namespace ptmk
