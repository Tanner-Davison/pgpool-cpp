#include <pqxx/pqxx>

#include <iostream>
#include <string>

int main() {
  try {
    // Connect to database
    pqxx::connection conn(
        "host=localhost port=5432 dbname=tanner user=tanner password=");
    if (conn.is_open()) {
      std::cout << "Connected to database: " << conn.dbname() << std::endl;
    }

    // Create a transaction
    pqxx::work txn(conn);

    // Run a simple query
    pqxx::result r = txn.exec("SELECT version()");

    // Print the result
    std::cout << "PostgreSQL version: " << r[0][0].as<std::string>()
              << std::endl;

    // Commit transaction
    txn.commit();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
