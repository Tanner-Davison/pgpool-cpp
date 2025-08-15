#pragma once
#include <pqxx/pqxx>

class ConnectionPool {
 public:
   ConnectionPool(const std::string& ppassword);

   void print_connection() const;
   ~ConnectionPool() = default;

 protected:
   const std::string password;
   const std::string connection_string = "host=localhost port=5432 dbname=tanner user=tanner password=";

 private:
   pqxx::connection              conn;
   std::unique_ptr<pqxx::work>   txn;
   std::shared_ptr<pqxx::result> r;
};
