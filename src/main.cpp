// Copyright (c) 2025 Tanner Davison. All Rights Reserved.
#include "ConnectionPool.hpp"
#include "TableCreator.hpp"
#include <iostream>
#include <string>
#include <thread>
/**
 * Basic demonstration of ConnectionPool with RAII connection handling.
 *
 * ConnectionPool configuration:
 *   - Host: localhost
 *   - Port: 5432
 *   - Database: tanner
 *   - User: tanner
 *   - Min connections: 1 (default)
 *   - Max connections: 10 (default)
 *
 * Note: This is a single-threaded example. The pool is thread-safe
 *       and can handle concurrent access in multi-threaded applications.
 */
int main() {
   try {
      const std::string connection_string = "host=localhost port=5432 dbname=tanner user=tanner";

      auto pool = std::make_shared<ConnectionPool>(connection_string, 2, 10);

      TableCreator tableCreator(pool);

      std::string schema =
          "id SERIAL PRIMARY KEY, "
          "name VARCHAR(100) NOT NULL, "
          "email VARCHAR(255) UNIQUE, "
          "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP";

      tableCreator.createTable("users", schema);

   } catch (const std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
   }
   unsigned int n = std::thread::hardware_concurrency();
   std::cout << "This system can run " << n << " threads in parallel\n";
   return 0;
}
