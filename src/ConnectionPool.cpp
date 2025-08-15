#include "ConnectionPool.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>

ConnectionPool::ConnectionPool(const std::string& ppassword) : password(ppassword), conn(connection_string + password) {
   try {
      txn = std::make_unique<pqxx::work>(conn);
      if (!conn.is_open()) {
         throw std::runtime_error("Failed to connect to database");
      }
      r = std::make_shared<pqxx::result>(txn->exec("SELECT version()"));
      print_connection();
      txn->commit();

   } catch (const pqxx::sql_error& e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
      std::cerr << "Query: " << e.query() << std::endl;
      throw;
   } catch (std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      throw;
   }
}
void ConnectionPool::print_connection() const {
   std::cout << "connected to: " << conn.dbname() << "\n";
   std::cout << "version: " << (*r)[0][0].as<std::string>() << std::endl;
}
