// Copyright (c) 2025 Tanner Davison. All Rights Reserved.
#include "DatabaseManager.hpp"
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
   std::cout << "Enter Password: ";
   std::string password;
   std::getline(std::cin, password);

   if (password.empty()) {
      std::cerr << "Password cannot be empty" << std::endl;
      return 1;
   }

   try {
      DatabaseManager db(password);
      db.tables().createTable("users",
                              "id SERIAL PRIMARY KEY, "
                              "username VARCHAR(50) NOT NULL, "
                              "email VARCHAR(100), "
                              "created_at TIMESTAMP DEFAULT NOW()");

      int user_id = db.data().insert("users", {"username", "email"}, {"john_doe", "john@example.com"});
      std::cout << "Inserted user with ID: " << user_id << std::endl;

      auto result = db.query().select("Select * FROM users");

      for (const auto& row : result) {
         std::cout << "User: " << row["username"].as<std::string>() << " - " << row["email"].as<std::string>()
                   << std::endl;
      }
      db.printPoolStats();

   } catch (const std::exception& e) {
      std::cerr << "Fatal error: " << e.what() << std::endl;
   }
   return 0;
}
