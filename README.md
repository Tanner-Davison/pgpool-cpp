# C++ PostgreSQL Connection Pool

A high-performance, thread-safe connection pool implementation for PostgreSQL using C++17 and libpqxx. This project provides efficient database connection management with automatic connection lifecycle handling and RAII-based resource management.

## 🚀 Features

- **Thread-Safe**: Full mutex protection with condition variables for concurrent access
- **RAII Design**: Automatic connection cleanup using RAII principles
- **Connection Pooling**: Configurable minimum and maximum connection limits
- **Smart Resource Management**: Uses `std::unique_ptr` and move semantics
- **Performance Optimized**: Efficient connection reuse and minimal overhead
- **Modern C++**: Built with C++17 features and best practices

## 🏗️ Architecture

### Core Components

#### `ConnectionPool` Class
The main connection pool manager that handles:
- Connection creation and initialization
- Connection lifecycle management
- Thread synchronization
- Pool size configuration

#### `ConnectionHandle` Nested Class
A RAII wrapper that provides:
- Automatic connection return to pool on destruction
- Move semantics for efficient resource transfer
- Pointer-like interface (`operator*` and `operator->`)
- Safe connection borrowing and returning

### Design Patterns

- **RAII (Resource Acquisition Is Initialization)**: Connections are automatically returned when handles go out of scope
- **Factory Pattern**: Connection creation is abstracted through the pool
- **RAII with Move Semantics**: Efficient resource transfer without copying
- **Thread-Safe Singleton Pattern**: Single pool instance with thread-safe access

## 📁 Project Structure

```
cpp-postgres/
├── include/
│   └── ConnectionPool.hpp      # Header file with class declarations
├── src/
│   ├── ConnectionPool.cpp      # Implementation of connection pool logic
│   └── main.cpp               # Example usage and testing
├── CMakeLists.txt             # Build configuration
├── .clang-format              # Code formatting rules
├── .gitignore                 # Git ignore patterns
└── README.md                  # This documentation
```

## 🔧 Prerequisites

- **Compiler**: C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **Build System**: CMake 3.20+
- **Dependencies**:
  - libpqxx (PostgreSQL C++ client library)
  - PostgreSQL development headers
- **Platform**: Cross-platform (Windows, Linux, macOS)

## 🛠️ Building the Project

### Using CMake

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Run the executable
./cpp-postgres
```

### CMake Configuration

The project automatically detects your platform and configures vcpkg paths:
- **Windows**: Uses `x64-mingw-dynamic` triplet
- **Linux**: Uses `x64-linux` triplet

## 📖 Usage Examples

### Basic Connection Pool Usage

```cpp
#include "ConnectionPool.hpp"
#include <iostream>

int main() {
    // Create connection pool with custom limits
    std::string conn_str = "host=localhost port=5432 dbname=mydb user=myuser password=mypass";
    ConnectionPool pool(conn_str, 5, 20); // min: 5, max: 20 connections
    
    // Get a connection from the pool
    {
        auto conn = pool.getConnection();
        
        // Use the connection (RAII ensures it's returned to pool)
        std::cout << "Active connections: " << pool.activeConnections() << std::endl;
        std::cout << "Total connections: " << pool.totalConnections() << std::endl;
        
        // Connection automatically returned when 'conn' goes out of scope
    }
    
    return 0;
}
```

### Advanced Usage with Transactions

```cpp
#include "ConnectionPool.hpp"
#include <pqxx/pqxx>

void performDatabaseOperation(ConnectionPool& pool) {
    auto conn = pool.getConnection();
    
    try {
        // Begin transaction
        pqxx::work txn(*conn);
        
        // Execute queries
        auto result = txn.exec("SELECT * FROM users WHERE active = true");
        
        // Process results
        for (const auto& row : result) {
            std::cout << "User: " << row[0].as<std::string>() << std::endl;
        }
        
        // Commit transaction
        txn.commit();
        
    } catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    
    // Connection automatically returned to pool
}
```

## 🔒 Thread Safety

The connection pool is fully thread-safe and supports concurrent access:

- **Mutex Protection**: All pool operations are protected by `std::mutex`
- **Condition Variables**: Efficient waiting for available connections
- **Atomic Operations**: Safe connection counting and state management
- **RAII Guarantees**: No race conditions during connection return

### Concurrent Usage Example

```cpp
#include <thread>
#include <vector>

void workerThread(ConnectionPool& pool, int threadId) {
    for (int i = 0; i < 100; ++i) {
        auto conn = pool.getConnection();
        
        // Simulate database work
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        // Connection automatically returned
    }
}

int main() {
    ConnectionPool pool("your_connection_string", 10, 50);
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(workerThread, std::ref(pool), i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return 0;
}
```

## 📊 Performance Characteristics

- **Connection Reuse**: Efficiently reuses existing connections
- **Minimal Overhead**: Low memory and CPU overhead per connection
- **Scalable**: Handles hundreds of concurrent requests efficiently
- **Memory Efficient**: Uses smart pointers for automatic cleanup

## 🧪 Testing and Validation

The project includes a simple test harness in `main.cpp` that demonstrates:
- Pool initialization
- Connection acquisition and return
- Active/total connection counting
- RAII behavior verification

## 🔍 Key Implementation Details

### Connection Lifecycle

1. **Initialization**: Pool creates minimum connections on startup
2. **Acquisition**: Threads wait for available connections or create new ones
3. **Usage**: Connections are marked as in-use during operation
4. **Return**: Automatic return via RAII when handles are destroyed
5. **Cleanup**: Unused connections remain available for reuse

### Memory Management

- **Smart Pointers**: `std::unique_ptr` for automatic connection cleanup
- **Move Semantics**: Efficient resource transfer without copying
- **RAII Guarantees**: Automatic cleanup on scope exit
- **No Memory Leaks**: All resources properly managed

## 🚨 Error Handling

The implementation includes robust error handling:
- **Exception Safety**: Strong exception guarantee for all operations
- **Resource Cleanup**: Automatic cleanup even in error conditions
- **Graceful Degradation**: Pool continues operating after individual connection failures

## 🔮 Future Enhancements

Potential improvements for future versions:
- Connection health checking and validation
- Configurable connection timeouts
- Connection load balancing
- Metrics and monitoring
- Connection encryption support
- Connection pooling strategies (LRU, FIFO, etc.)

## 📝 License

(C) 2025 Tanner Davidson. All Rights Reserved.

## 🤝 Contributing

This is a personal project, but suggestions and improvements are welcome. Please ensure any contributions maintain the existing code style and architecture principles.

## 📚 Dependencies

- **libpqxx**: PostgreSQL C++ client library
- **Standard C++ Library**: C++17 features and containers
- **CMake**: Build system and dependency management

## 🐛 Troubleshooting

### Common Issues

1. **Connection Failures**: Verify PostgreSQL is running and credentials are correct
2. **Build Errors**: Ensure C++17 compiler and libpqxx are properly installed
3. **Runtime Errors**: Check connection string format and database accessibility

### Debug Information

The pool provides debugging information during initialization and operation:
- Connection pool size on startup
- Active connection counts
- Total connection counts

## 📞 Support

For questions or issues related to this project, please refer to the code comments and implementation details in the source files.
Contact: tanner.davison95@gmail.com
