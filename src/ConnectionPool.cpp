#include "ConnectionPool.hpp"
#include <chrono>
#include <iostream>

ConnectionPool::ConnectionPool(const std::string& conn_str, size_t min_conns, size_t max_conns)
    : connection_string(conn_str), min_connections(min_conns), max_connections(max_conns) {
   for (size_t i = 0; i < min_conns; ++i) {
      createConnection();
   }

   std::cout << "Connection pool initialized with " << min_connections << " connections" << std::endl;
}

void ConnectionPool::createConnection() {
   auto conn = std::make_unique<pqxx::connection>(connection_string);
   connections.push_back({std::move(conn), std::chrono::steady_clock::now(), false});
   available_indices.push(connections.size() - 1); // tracks count of Pooled connections
}

// ConnectionHandle consturctor
ConnectionPool::ConnectionHandle::ConnectionHandle(pqxx::connection* c, ConnectionPool* p, size_t idx)
    : conn(c), pool(p), index(idx) {}

// ConnectionHandle  move constructor
ConnectionPool::ConnectionHandle::ConnectionHandle(ConnectionHandle&& other) noexcept
    : conn(other.conn), pool(other.pool), index(other.index) {
   other.conn = nullptr;
   other.pool = nullptr;
};

void ConnectionPool::returnConnection(size_t index) {
   std::lock_guard<std::mutex> lock(pool_mutex);
   connections[index].in_use = false;
   available_indices.push(index);
   pool_cv.notify_one();
}

ConnectionPool::ConnectionHandle::~ConnectionHandle() {
   if (pool && conn) {
      pool->returnConnection(index);
   }
}

ConnectionPool::ConnectionHandle ConnectionPool::getConnection() { // Returns a ConnectionHandle
   std::unique_lock<std::mutex> lock(pool_mutex);

   // wait for availbalbe connections or create a new one
   pool_cv.wait(lock, [this] { return !available_indices.empty() || connections.size() < max_connections; });
   if (available_indices.empty() && connections.size() < max_connections) {
      createConnection();
   }
   size_t index = available_indices.front(); // take a free indice from available_indices
   available_indices.pop();                  // remove from queue
   connections[index].in_use    = true;
   connections[index].last_used = std::chrono::steady_clock::now();

   return ConnectionHandle(connections[index].conn.get(), this, index);
}

size_t ConnectionPool::activeConnections() const { // using mutable do not const
   std::lock_guard<std::mutex> lock(pool_mutex);
   return connections.size() - available_indices.size();
}
size_t ConnectionPool::totalConnections() const { // using mutable do not const
   std::lock_guard<std::mutex> lock(pool_mutex);
   return connections.size();
}
