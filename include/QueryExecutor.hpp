#pragma once
#include "DBOperation.hpp"
#include <cerrno>
#include <iostream>
#include <pqxx/pqxx>

class QueryExecutor : public DBOperation {
 public:
   using DBOperation::DBOperation;
   pqxx::result select(const std::string& query);

   pqxx::result selectPrepared(const std::string& table, const std::string& condition_column, const std::string& value);
};
