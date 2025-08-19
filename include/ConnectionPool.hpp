#pragma once
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>
#include <string>
#include <vector>

class ConnectionPool {
 private:
   struct PooledConnection {
      std::unique_ptr<pqxx::connection>     conn;
      std::chrono::steady_clock::time_point last_used;

      bool in_use;
   };
   std::vector<PooledConnection> connections;
   std::queue<size_t>            available_indices;
   mutable std::mutex            pool_mutex;
   std::condition_variable       pool_cv;

   const std::string connection_string;
   const size_t      max_connections;
   const size_t      min_connections;

   void createConnection();

 public:
   ConnectionPool(const std::string& conn_str, size_t min_conns = 1, size_t max_conns = 10);

   /* <-----------------------ConnectionHandle NESTED CLASS START ---------------------->*/
   class ConnectionHandle {
    private:
      pqxx::connection* conn;  // one specific connection
      ConnectionPool*   pool;  // reference back to pool
      size_t            index; // which connection we borrowed

    public:
      ConnectionHandle(pqxx::connection* c, ConnectionPool* p, size_t idx);
      ConnectionHandle(ConnectionHandle&& other) noexcept;
      ~ConnectionHandle();

      pqxx::connection& operator*() { // overloaded conn
         return *conn;
      };
      pqxx::connection* operator->() { // overloaded conn
         return conn;
      };
      // delete copy constructor and assignment
      ConnectionHandle(const ConnectionHandle&)            = delete;
      ConnectionHandle& operator=(const ConnectionHandle&) = delete;
   };

   /* <-----------------------ConnectionHandle NESTED CLASS END---------------------->*/

   ConnectionHandle getConnection(); // returns a ConecctionHandle

 private:
   void returnConnection(size_t index) {
      std::lock_guard<std::mutex> lock(pool_mutex);
      connections[index].in_use = false;
      available_indices.push(index);
      pool_cv.notify_one();
   }

 public:
   size_t activeConnections() const;
   size_t totalConnections() const;
};
