// (C) 2025 Tanner Daviosn. All Rights Reserved
#include "ConnectionPool.hpp"
#include <iostream>
#include <string>

int main() {
   std::string password;
   getline(std::cin, password);

   if (!password.empty()) {
      ConnectionPool tanner(password);
   }

   return 0;
}
