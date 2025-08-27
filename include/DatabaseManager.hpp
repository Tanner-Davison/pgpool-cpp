#pragma once

#include "DataModifier.hpp"
#include "QueryExecutor.hpp"
#include "TableCreator.hpp"
class DatabaseManager {
 private:
   std::shared_ptr<ConnectionPool> pool;
   std::unique_ptr<TableCreator>   table_ops;
   std::unique_ptr<QueryExecutor>  query_ops;
   std::unique_ptr<DataModifier>   data_ops;

 public:
   DatabaseManager(const std::string& password,
                   const std::string& host            = "localhost",
                   const std::string& dbname          = "tanner",
                   const std::string& user            = "tanner",
                   size_t             min_connections = 2,
                   size_t             max_connections = 10);
   void testConnection();

   TableCreator&  tables();
   QueryExecutor& query();
   DataModifier&  data();

   void printPoolStats();
};
