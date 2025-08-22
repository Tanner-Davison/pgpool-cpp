#include "TableCreator.hpp"
#include <cerrno>
#include <iostream>
#include <pqxx/pqxx>

void TableCreator::createTable(const std::string& table_name, const std::string& schema) {
   auto conn_handle = pool->getConnection();
   try {
      pqxx::work  txn(*conn_handle);
      std::string query = "CREATE TABLE IF NOT EXISTS " + txn.esc(table_name) + " (" + schema + ")";
      txn.exec(query);
      txn.commit();
      std::cout << "'Table '" << table_name << "' created successfully'" << std::endl;
   } catch (const pqxx::sql_error& e) {
      std::cerr << "SQL Error creating table: " << e.what() << std::endl;
      throw;
   }
}
void TableCreator::dropTable(const std::string& table_name) {
   auto conn_hanlde = pool->getConnection();

   try {
      pqxx::work txn(*conn_hanlde);
      txn.exec("DROP TABLE IF EXISTS " + txn.esc(table_name));
      txn.commit();
      std::cout << "'Table '" << table_name << "' dropped'" << std::endl;
   } catch (pqxx::sql_error& e) {
      std::cerr << "SQL Error dropping table: " << e.what() << std::endl;
   }
}
