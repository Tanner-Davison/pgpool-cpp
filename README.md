# C++ PostgreSQL Connection Pool

A high-performance, thread-safe connection pool implementation for PostgreSQL using C++17 and libpqxx. This project provides efficient database connection management with automatic connection lifecycle handling, RAII-based resource management, and a comprehensive database operations interface.

## 🚀 Features

- **Thread-Safe**: Full mutex protection with condition variables for concurrent access
- **RAII Design**: Automatic connection cleanup using RAII principles
- **Connection Pooling**: Configurable minimum and maximum connection limits
- **Smart Resource Management**: Uses `std::unique_ptr` and move semantics
- **Performance Optimized**: Efficient connection reuse and minimal overhead
- **Modern C++**: Built with C++17 features and best practices
- **Comprehensive Database Operations**: Table creation, data modification, and query execution
- **Unified Database Interface**: Single `DatabaseManager` class for all database operations

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

#### `DatabaseManager` Class
A unified interface that provides:
- Connection pool management
- Table operations (create/drop tables)
- Query execution (SELECT operations)
- Data modification (INSERT/UPDATE operations)
- Connection testing and pool statistics

#### `DBOperation` Base Class
Abstract base class for database operations:
- Shared connection pool access
- Common interface for all database operations

#### `TableCreator` Class
Handles table management operations:
- Create tables with custom schemas
- Drop existing tables
- Inherits from `DBOperation` for pool access

#### `QueryExecutor` Class
Manages database queries:
- Execute SELECT queries
- Prepared statement support for safe parameterized queries
- Inherits from `DBOperation` for pool access

#### `DataModifier` Class
Handles data modification operations:
- INSERT operations with column-value pairs
- UPDATE operations with WHERE conditions
- Inherits from `DBOperation` for pool access

### Design Patterns

- **RAII (Resource Acquisition Is Initialization)**: Connections are automatically returned when handles go out of scope
- **Factory Pattern**: Connection creation is abstracted through the pool
- **RAII with Move Semantics**: Efficient resource transfer without copying
- **Thread-Safe Singleton Pattern**: Single pool instance with thread-safe access
- **Inheritance Hierarchy**: Clean separation of concerns with `DBOperation` base class
- **Composition**: `DatabaseManager` composes all operation classes

## 📁 Project Structure

```
pgpool-cpp/
├── include/
│   ├── ConnectionPool.hpp      # Connection pool class declarations
│   ├── DatabaseManager.hpp     # Main database interface
│   ├── DBOperation.hpp        # Base class for database operations
│   ├── TableCreator.hpp       # Table management operations
│   ├── QueryExecutor.hpp      # Query execution operations
│   └── DataModifier.hpp       # Data modification operations
├── src/
│   ├── ConnectionPool.cpp      # Connection pool implementation
│   ├── DatabaseManager.cpp     # Database manager implementation
│   ├── DBOperation.hpp        # Base class implementation
│   ├── TableCreator.cpp        # Table operations implementation
│   ├── QueryExecutor.cpp       # Query execution implementation
│   ├── DataModifier.cpp        # Data modification implementation
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
./pgpool-cpp
```

### CMake Configuration

The project automatically detects your platform and configures vcpkg paths:
- **Windows**: Uses `x64-mingw-dynamic` triplet
- **Linux**: Uses `x64-linux` triplet

## 📖 Usage Examples

### Basic Database Manager Usage

```cpp
#include "DatabaseManager.hpp"
#include <iostream>

int main() {
    std::cout << "Enter Password: ";
    std::string password;
    std::getline(std::cin, password);

    try {
        // Create database manager with custom configuration
        DatabaseManager db(password, "localhost", "mydb", "myuser", 2, 10);
        
        // Test the connection
        db.testConnection();
        
        // Print pool statistics
        db.printPoolStats();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### Table Management

```cpp
#include "DatabaseManager.hpp"

void manageTables(DatabaseManager& db) {
    // Create a new table
    db.tables().createTable("users",
        "id SERIAL PRIMARY KEY, "
        "username VARCHAR(50) NOT NULL, "
        "email VARCHAR(100), "
        "created_at TIMESTAMP DEFAULT NOW()");
    
    // Drop a table if needed
    // db.tables().dropTable("old_table");
}
```

### Data Operations

```cpp
#include "DatabaseManager.hpp"

void performDataOperations(DatabaseManager& db) {
    // Insert data
    int user_id = db.data().insert("users", 
        {"username", "email"}, 
        {"john_doe", "john@example.com"});
    
    std::cout << "Inserted user with ID: " << user_id << std::endl;
    
    // Update data
    size_t updated_rows = db.data().update("users", 
        "email", "newemail@example.com",
        "username", "john_doe");
    
    std::cout << "Updated " << updated_rows << " rows" << std::endl;
}
```

### Query Execution

```cpp
#include "DatabaseManager.hpp"

void executeQueries(DatabaseManager& db) {
    // Simple SELECT query
    auto result = db.query().select("SELECT * FROM users");
    
    for (const auto& row : result) {
        std::cout << "User: " << row["username"].as<std::string>() 
                  << " - " << row["email"].as<std::string>() << std::endl;
    }
    
    // Prepared statement for safe parameterized queries
    auto filtered_result = db.query().selectPrepared("users", "username", "john_doe");
    
    for (const auto& row : filtered_result) {
        std::cout << "Found user: " << row["email"].as<std::string>() << std::endl;
    }
}
```

### Complete Working Example

```cpp
#include "DatabaseManager.hpp"
#include <iostream>

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
        
        // Create a table
        db.tables().createTable("users",
            "id SERIAL PRIMARY KEY, "
            "username VARCHAR(50) NOT NULL, "
            "email VARCHAR(100), "
            "created_at TIMESTAMP DEFAULT NOW()");

        // Insert data
        int user_id = db.data().insert("users", 
            {"username", "email"}, 
            {"john_doe", "john@example.com"});
        std::cout << "Inserted user with ID: " << user_id << std::endl;

        // Query data
        auto result = db.query().select("SELECT * FROM users");
        for (const auto& row : result) {
            std::cout << "User: " << row["username"].as<std::string>() 
                      << " - " << row["email"].as<std::string>() << std::endl;
        }
        
        // Show pool statistics
        db.printPoolStats();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    
    return 0;
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

void workerThread(DatabaseManager& db, int threadId) {
    for (int i = 0; i < 100; ++i) {
        // Each operation gets its own connection from the pool
        auto result = db.query().select("SELECT COUNT(*) FROM users");
        std::cout << "Thread " << threadId << " count: " 
                  << result[0][0].as<int>() << std::endl;
    }
}

int main() {
    DatabaseManager db("password", "localhost", "mydb", "user", 10, 50);
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(workerThread, std::ref(db), i);
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
- **Optimized Queries**: Prepared statements and parameterized queries for security and performance

## 🧪 Testing and Validation

The project includes a comprehensive test harness in `main.cpp` that demonstrates:
- Database connection and pool initialization
- Table creation and management
- Data insertion and modification
- Query execution and result processing
- Pool statistics and monitoring
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

### Database Operation Hierarchy

```
DatabaseManager
├── ConnectionPool (shared_ptr)
├── TableCreator (unique_ptr)
├── QueryExecutor (unique_ptr)
└── DataModifier (unique_ptr)
```

Each operation class inherits from `DBOperation` and shares access to the connection pool, ensuring consistent resource management across all database operations.

## 🚨 Error Handling

The implementation includes robust error handling:
- **Exception Safety**: Strong exception guarantee for all operations
- **Resource Cleanup**: Automatic cleanup even in error conditions
- **Graceful Degradation**: Pool continues operating after individual connection failures
- **SQL Error Handling**: Specific handling for PostgreSQL errors
- **Input Validation**: Parameter validation for all public methods

## 🔮 Future Enhancements

Potential improvements for future versions:
- Connection health checking and validation
- Configurable connection timeouts
- Connection load balancing
- Metrics and monitoring
- Connection encryption support
- Connection pooling strategies (LRU, FIFO, etc.)
- Transaction management utilities
- Batch operation support
- Connection pool configuration file support

## 📝 License

(C) 2025 Tanner Davison. All Rights Reserved.

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
4. **Table Creation Errors**: Verify database permissions and schema syntax
5. **Query Execution Errors**: Check SQL syntax and table existence

### Debug Information

The pool provides debugging information during initialization and operation:
- Connection pool size on startup
- Active connection counts
- Total connection counts
- Table creation/drop confirmations
- Query result row counts

## 📞 Support

For questions or issues related to this project, please refer to the code comments and implementation details in the source files.
Contact: tanner.davison95@gmail.com
