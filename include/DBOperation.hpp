
#include "ConnectionPool.hpp"
#include <memory>
class DBOperation {
 protected:
   std::shared_ptr<ConnectionPool> pool;

 public:
   explicit DBOperation(std::shared_ptr<ConnectionPool> connection_pool) : pool(connection_pool) {}
   virtual ~DBOperation() = default;
};
