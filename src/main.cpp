// Copyright (c) 2025 Tanner Davison. All Rights Reserved.
#include "ConnectionPool.hpp"

#include <iostream>
#include <string>

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
      // Connection string format for PostgreSQL
      const std::string connection_string = "host=localhost port=5432 dbname=tanner user=tanner";

      // Create pool with default settings (1 min, 10 max connections)
      ConnectionPool pool(connection_string);

      std::cout << "=== Connection Pool Demo ===\n\n";

      // Demonstrate RAII connection management
      {
         std::cout << "Acquiring connections...\n";
         auto conn1 = pool.getConnection();
         auto conn2 = pool.getConnection();

         std::cout << "Active connections: " << pool.activeConnections() << "\n";
         std::cout << "Total connections:  " << pool.totalConnections() << "\n\n";

         // Optional: Actually use the connections
         if (conn1->is_open()) {
            std::cout << "Connection 1: Connected to " << conn1->dbname() << "\n";
         }
         if (conn2->is_open()) {
            std::cout << "Connection 2: Connected to " << conn2->dbname() << "\n";
         }

         std::cout << "\nConnections will be returned to pool at end of scope...\n";
      } // Connections automatically returned here

      std::cout << "\nAfter scope:\n";
      std::cout << "Active connections: " << pool.activeConnections() << "\n";
      std::cout << "Total connections:  " << pool.totalConnections() << "\n";

   } catch (const pqxx::broken_connection& e) {
      std::cerr << "Database connection failed: " << e.what() << "\n";
      return 1;
   } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
   }

   std::cout << "\n=== Demo completed successfully ===\n";
   return 0;
}
