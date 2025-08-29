#include "DatabaseManager.hpp"
#include "QueryExecutor.hpp"
#include <memory>

DatabaseManager::DatabaseManager(const std::string& password,
                                 const std::string& host,
                                 int                m_port,
                                 const std::string& dbname,
                                 const std::string& user,
                                 size_t             min_connections,
                                 size_t             max_connections) {
   std::string conn_string = "host=" + host + " port=" + std::to_string(m_port) + " dbname=" + dbname +
                             " user=" + user + " password=" + password;

   pool = std::make_shared<ConnectionPool>(conn_string, min_connections, max_connections);

   table_ops = std::make_unique<TableCreator>(pool);
   query_ops = std::make_unique<QueryExecutor>(pool);
   data_ops  = std::make_unique<DataModifier>(pool);

   testConnection();
}
void DatabaseManager::testConnection() {
   try {
      auto result = query_ops->select("SELECT version()");
      std::cout << "Database version: " << result[0][0].as<std::string>() << std::endl;

   } catch (const std::exception& e) {
      std::cerr << "Connection test Failed: " << e.what() << std::endl;
      throw;
   }
}

size_t DatabaseManager::getActiveConnections() const {
   return pool->activeConnections();
}

size_t DatabaseManager::getTotalConnections() const {
   return pool->totalConnections();
}
TableCreator& DatabaseManager::tables() {
   return *table_ops;
}
QueryExecutor& DatabaseManager::query() {
   return *query_ops;
}
DataModifier& DatabaseManager::data() {
   return *data_ops;
}

void DatabaseManager::printPoolStats() {
   std::cout << "Pool stats -Active: ";
   std::cout << pool->activeConnections() << " / Total: ";
   std::cout << pool->totalConnections() << std::endl;
}
