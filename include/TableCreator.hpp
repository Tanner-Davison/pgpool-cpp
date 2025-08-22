#pragma once
#include "DBOperation.hpp"
#include <string>

class TableCreator : public DBOperation {
 public:
   using DBOperation::DBOperation; // Inherits contructors from DBOperation
   void createTable(const std::string& table_name, const std::string& schema);
   void dropTable(const std::string& table_name);
};
