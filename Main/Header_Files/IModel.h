#pragma once

#include <vector>

#include <pqxx/pqxx>

namespace ptmk {

class IModel {
public:
  static void CreateTable();
  static void BulkInsert(pqxx::connection &, std::vector<IModel> &);
  static std::vector<IModel> selectAll(pqxx::connection &);

  virtual void insert(pqxx::connection &) = 0;
  virtual void update(pqxx::connection &) = 0;
  virtual void insertUpdate(pqxx::connection &) = 0;

  virtual ~IModel() {}
};

} // namespace ptmk
