// (C) 2025 Tanner Daviosn. All Rights Reserved
#include "ConnectionPool.hpp"
#include <iostream>
#include <string>
/*
 * THIS IS A BASIC IMPLEMENTATION OF CONNECTING AND CREATING
 * A CONNECTION HANDLER (SINGLE THREADED IMPLEMENTATION)
 *
 *    ConnectionPool implmentation takes->
 * 1. Host
 * 2. Port
 * 3. Database name
 *
 */

int main() {
   std::string connection_string = "host=localhost port=5432 dbname=tanner user=tanner ";

   ConnectionPool pool(connection_string);
   {
      auto conn  = pool.getConnection();
      auto conn2 = pool.getConnection();
      std::cout << "active connections: " << std::endl;
      std::cout << pool.activeConnections() << std::endl;

      std::cout << "total Connections: " << std::endl;
      std::cout << pool.totalConnections() << std::endl;
   }

   return 0;
}
