#pragma once
#include "DBOperation.hpp"

class DataModifier : public DBOperation {
 public:
   using DBOperation::DBOperation;
   int    insert(const std::string& table, const std::vector<std::string>& columns,
                 const std::vector<std::string>& values);
   size_t update(const std::string& table, const std::string& set_column, const std::string& set_value,
                 const std::string& where_column, const std::string& where_value);
};
