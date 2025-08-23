// Copyright (c) 2025 Tanner Davison. All Rights Reserved.
#pragma once

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <pqxx/pqxx>

/**
 * ConnectionPool
 *   ├─[owns]→ vector<PooledConnection>
 *   │            └─[contains]→ pqxx::connection
 *   │
 *   └─[creates]→ ConnectionHandle
 *                  └─[borrows]→ PooledConnection
 *                  └─[references]→ ConnectionPool
 */
class ConnectionPool {
 public:
   // Forward declare nested class
   class ConnectionHandle;

   explicit ConnectionPool(const std::string& conn_str, size_t min_conns = 1, size_t max_conns = 10);

   ConnectionHandle getConnection();
   size_t           activeConnections() const;
   size_t           totalConnections() const;

   /* <-----------------------ConnectionHandle NESTED CLASS ---------------------->*/
   class ConnectionHandle {
    public:
      ConnectionHandle(pqxx::connection* c, ConnectionPool* p, size_t idx);
      ConnectionHandle(ConnectionHandle&& other) noexcept;
      ~ConnectionHandle();

      // Overloaded Accessors
      pqxx::connection& operator*() {
         return *conn;
      }
      pqxx::connection* operator->() {
         return conn;
      }

      // Deleted operations
      ConnectionHandle(const ConnectionHandle&)            = delete; // deleted copy constructor
      ConnectionHandle& operator=(const ConnectionHandle&) = delete; // deleted copy assaignment
      ConnectionHandle& operator=(ConnectionHandle&&)      = delete; // deleted move assaignment

    private:
      pqxx::connection* conn;  // One specific connection
      ConnectionPool*   pool;  // Reference back to pool
      size_t            index; // Which connection we borrowed
   };

   /* <-------------------ConnectionHandle END Of NESTED CLASS ----------------->*/

 private:
   struct PooledConnection {
      std::unique_ptr<pqxx::connection>     conn;
      std::chrono::steady_clock::time_point last_used;
      bool                                  in_use;
   };

   // Private Member variables
   std::vector<PooledConnection> connections;
   std::queue<size_t>            available_indices;
   mutable std::mutex            pool_mutex; // mutable for const methods
   std::condition_variable       pool_cv;

   const std::string connection_string;
   const size_t      max_connections;
   const size_t      min_connections;

   // Private methods
   void createConnection();
   void returnConnection(size_t index); // Friend access for ConnectionHandle

   friend class ConnectionHandle; // Allow handle to call returnConnection
};
