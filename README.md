# PgPool Manager - Qt6 PostgreSQL Database Management Application

A modern, feature-rich PostgreSQL database management application built with C++17, Qt6, and libpqxx. This project provides an intuitive graphical interface for database operations, connection pool management, and comprehensive database administration tools.

## 🚀 Features

- **Modern Qt6 GUI**: Clean, responsive user interface built with Qt6
- **Database Connection Management**: Easy connection setup with configurable parameters
- **Connection Pooling**: High-performance, thread-safe connection pool with automatic lifecycle management
- **Table Management**: Create, drop, and manage database tables through the GUI
- **Data Operations**: Insert, update, and query data with visual feedback
- **Query Execution**: Execute custom SQL queries with result display
- **Connection Pool Testing**: Built-in tools to test and monitor connection pool performance
- **Cross-Platform**: Runs on Windows, Linux, and macOS
- **Modern C++**: Built with C++17 features and best practices
- **RAII Design**: Automatic resource cleanup using RAII principles

## 🖥️ Application Screenshot

![PostgreSQL Connection Pool Manager](screenshots/app-screenshot.jpg)

*The main interface showing database connection configuration, query execution, results display, and real-time logging*

## 🏗️ Architecture

### Core Components

#### `MainWindow` Class
The main application window that provides:
- Database connection configuration interface
- Connection pool management and testing
- Table selection and management tools
- Query execution interface
- Data insertion dialog integration
- Real-time connection status monitoring

#### `InsertDialog` Class
A dedicated dialog for data insertion that offers:
- Dynamic table column detection
- Multi-row data insertion
- Add/remove row functionality
- Type-aware data input
- Bulk insert operations

#### `ConnectionPool` Class
The underlying connection pool manager that handles:
- Connection creation and initialization
- Connection lifecycle management
- Thread synchronization
- Pool size configuration
- Connection health monitoring

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

- **Model-View-Controller (MVC)**: Clean separation between UI, data, and business logic
- **RAII (Resource Acquisition Is Initialization)**: Connections are automatically returned when handles go out of scope
- **Factory Pattern**: Connection creation is abstracted through the pool
- **RAII with Move Semantics**: Efficient resource transfer without copying
- **Thread-Safe Singleton Pattern**: Single pool instance with thread-safe access
- **Inheritance Hierarchy**: Clean separation of concerns with `DBOperation` base class
- **Composition**: `DatabaseManager` composes all operation classes
- **Signal-Slot Pattern**: Qt's event-driven architecture for UI interactions

## 📁 Project Structure

```
pgpool-cpp/
├── include/
│   ├── MainWindow.hpp         # Main application window
│   ├── InsertDialog.hpp       # Data insertion dialog
│   ├── ConnectionPool.hpp     # Connection pool class declarations
│   ├── DatabaseManager.hpp    # Main database interface
│   ├── DBOperation.hpp        # Base class for database operations
│   ├── TableCreator.hpp       # Table management operations
│   ├── QueryExecutor.hpp      # Query execution operations
│   └── DataModifier.hpp       # Data modification operations
├── src/
│   ├── main.cpp               # Application entry point
│   ├── MainWindow.cpp         # Main window implementation
│   ├── InsertDialog.cpp       # Insert dialog implementation
│   ├── ConnectionPool.cpp     # Connection pool implementation
│   ├── DatabaseManager.cpp    # Database manager implementation
│   ├── DBOperation.hpp        # Base class implementation
│   ├── TableCreator.cpp       # Table operations implementation
│   ├── QueryExecutor.cpp      # Query execution implementation
│   └── DataModifier.cpp       # Data modification implementation
├── build/                     # Build artifacts and CMake files
├── CMakeLists.txt            # Build configuration
├── setup-qt.sh               # Qt6 setup script for Linux
├── .clang-format             # Code formatting rules
├── .gitignore                # Git ignore patterns
└── README.md                 # This documentation
```

## 🔧 Prerequisites

- **Compiler**: C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **Build System**: CMake 3.20+
- **Dependencies**:
  - Qt6 (Core, Widgets, Network, Sql components)
  - libpqxx (PostgreSQL C++ client library)
  - PostgreSQL development headers
- **Platform**: Cross-platform (Windows, Linux, macOS)

## 🛠️ Building the Project

### Prerequisites Setup

#### Linux (Ubuntu/Debian)
```bash
# Run the setup script
chmod +x setup-qt.sh
./setup-qt.sh
```

#### Windows
1. Install Qt6 via Qt installer or vcpkg
2. Install vcpkg and libpqxx
3. Ensure MinGW or MSVC compiler is available

#### macOS
```bash
# Install Qt6 via Homebrew
brew install qt6

# Install vcpkg and libpqxx
# Follow similar steps as Linux
```

### Building with CMake

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Run the application
./pgpool-cpp
```

### CMake Configuration

The project automatically detects your platform and configures vcpkg paths:
- **Windows**: Uses `x64-mingw-dynamic` triplet
- **Linux**: Uses `x64-linux` triplet
- **Qt6 Integration**: Automatic MOC, RCC, and UIC processing

## 📖 Usage Guide

### Application Overview

PgPool Manager provides an intuitive interface for PostgreSQL database management:

1. **Connection Setup**: Configure database connection parameters
2. **Connection Testing**: Verify connectivity and pool functionality
3. **Table Management**: Create and manage database tables
4. **Data Operations**: Insert, query, and modify data
5. **Query Execution**: Run custom SQL queries
6. **Pool Monitoring**: Monitor connection pool performance

### Main Window Features

#### Database Connection
- **Host**: Database server address (default: localhost)
- **Port**: Database port (default: 5432)
- **Database**: Target database name
- **Username**: Database user credentials
- **Password**: Secure password input
- **Pool Size**: Connection pool configuration

#### Database Operations
- **Connect**: Establish database connection
- **Test Pool**: Verify connection pool functionality
- **Refresh Tables**: Update table list
- **Execute Query**: Run custom SQL queries
- **Insert Data**: Open data insertion dialog

#### Results Display
- **Query Results**: Tabular display of query results
- **Log Output**: Real-time operation logging
- **Status Bar**: Connection and operation status

### Data Insertion Dialog

The Insert Dialog provides a user-friendly way to insert data:

1. **Table Selection**: Choose target table from dropdown
2. **Column Detection**: Automatic column and type detection
3. **Data Input**: Type-aware input fields
4. **Row Management**: Add/remove rows for bulk operations
5. **Insert Execution**: Execute insert operations with feedback

### Example Workflow

1. **Launch Application**: Start PgPool Manager
2. **Configure Connection**: Enter database credentials
3. **Test Connection**: Verify connectivity
4. **Create Table**: Use table management tools
5. **Insert Data**: Use the Insert Dialog
6. **Query Data**: Execute SELECT queries
7. **Monitor Pool**: Check connection pool status

## 🔒 Thread Safety

The connection pool is fully thread-safe and supports concurrent access:

- **Mutex Protection**: All pool operations are protected by `std::mutex`
- **Condition Variables**: Efficient waiting for available connections
- **Atomic Operations**: Safe connection counting and state management
- **RAII Guarantees**: No race conditions during connection return
- **Qt Thread Safety**: UI operations are properly synchronized

## 📊 Performance Characteristics

- **Connection Reuse**: Efficiently reuses existing connections
- **Minimal Overhead**: Low memory and CPU overhead per connection
- **Scalable**: Handles hundreds of concurrent requests efficiently
- **Memory Efficient**: Uses smart pointers for automatic cleanup
- **Optimized Queries**: Prepared statements and parameterized queries for security and performance
- **Responsive UI**: Non-blocking database operations with progress feedback

## 🧪 Testing and Validation

The application includes comprehensive testing capabilities:

- **Connection Testing**: Verify database connectivity
- **Pool Testing**: Test connection pool functionality
- **Table Operations**: Validate table creation and management
- **Data Operations**: Test insert, query, and update operations
- **Query Execution**: Validate custom SQL execution
- **Error Handling**: Comprehensive error handling and user feedback

## 🔍 Key Implementation Details

### Qt6 Integration

- **MOC Processing**: Automatic meta-object compilation
- **Signal-Slot Connections**: Event-driven architecture
- **Widget Management**: Dynamic UI creation and management
- **Cross-Platform**: Native look and feel on all platforms

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
- **Qt Object Hierarchy**: Proper parent-child relationships

## 🚨 Error Handling

The implementation includes robust error handling:
- **Exception Safety**: Strong exception guarantee for all operations
- **Resource Cleanup**: Automatic cleanup even in error conditions
- **Graceful Degradation**: Pool continues operating after individual connection failures
- **SQL Error Handling**: Specific handling for PostgreSQL errors
- **Input Validation**: Parameter validation for all public methods
- **User Feedback**: Clear error messages and status updates
- **Logging**: Comprehensive operation logging for debugging

## 🔮 Future Enhancements

Potential improvements for future versions:
- **Advanced Query Builder**: Visual query construction interface
- **Data Export/Import**: CSV, JSON, and other format support
- **Schema Visualization**: Database structure diagrams
- **Performance Monitoring**: Real-time performance metrics
- **Backup/Restore**: Database backup and restoration tools
- **User Management**: Role-based access control
- **Plugin System**: Extensible architecture for custom functionality
- **Dark Mode**: Theme customization options
- **Keyboard Shortcuts**: Power user productivity features
- **Multi-Database Support**: Connection to multiple databases simultaneously

## 📝 License

(C) 2025 Tanner Davison. All Rights Reserved.

## 🤝 Contributing

This is a personal project, but suggestions and improvements are welcome. Please ensure any contributions maintain the existing code style and architecture principles.

## 📚 Dependencies

- **Qt6**: Modern C++ application framework
  - Core: Core non-GUI functionality
  - Widgets: GUI components
  - Network: Network operations
  - Sql: Database connectivity
- **libpqxx**: PostgreSQL C++ client library
- **Standard C++ Library**: C++17 features and containers
- **CMake**: Build system and dependency management

## 🐛 Troubleshooting

### Common Issues

1. **Qt6 Not Found**: Ensure Qt6 is properly installed and CMake can locate it
2. **Connection Failures**: Verify PostgreSQL is running and credentials are correct
3. **Build Errors**: Ensure C++17 compiler and all dependencies are properly installed
4. **Runtime Errors**: Check connection string format and database accessibility
5. **Table Creation Errors**: Verify database permissions and schema syntax
6. **Query Execution Errors**: Check SQL syntax and table existence
7. **UI Rendering Issues**: Verify Qt6 installation and graphics drivers

### Debug Information

The application provides comprehensive debugging information:
- Connection pool size on startup
- Active connection counts
- Total connection counts
- Table creation/drop confirmations
- Query result row counts
- Real-time operation logging
- Connection status monitoring

### Platform-Specific Notes

- **Linux**: Ensure proper Qt6 installation and graphics drivers
- **Windows**: Verify MinGW/MSVC compatibility and Qt6 paths
- **macOS**: Check Homebrew Qt6 installation and permissions

## 🔧 Debugging & Getting Things Running

### Build Troubleshooting

#### CMake Configuration Issues
```bash
# If CMake can't find Qt6
export CMAKE_PREFIX_PATH="/path/to/qt6:$CMAKE_PREFIX_PATH"

# For vcpkg integration issues
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Clean build if you encounter strange errors
cd build
ninja clean
cmake ..
ninja
```

#### Compilation Errors
```bash
# Check compiler version (C++17 required)
g++ --version
clang++ --version

# Verify Qt6 installation
qmake6 --version
qt6-config --version

# Check if all dependencies are found
cmake .. -LAH | grep -E "(Qt6|libpqxx|PostgreSQL)"
```

### Runtime Troubleshooting

#### Display Issues (Linux/WSL)
```bash
# Check if display is set
echo $DISPLAY

# Set display for local terminal
export DISPLAY=:0

# For WSL, you might need X11 forwarding
export DISPLAY=localhost:0.0

# Check available Qt platforms
./pgpool-cpp -platform help

# Force specific platform if needed
./pgpool-cpp -platform xcb
./pgpool-cpp -platform wayland
```

#### Qt Platform Issues
```bash
# List available Qt platforms
./pgpool-cpp -platform help

# Common platforms:
# - xcb (X11 on Linux)
# - wayland (Wayland on Linux)
# - windows (Windows)
# - cocoa (macOS)

# Force platform if auto-detection fails
./pgpool-cpp -platform xcb
```

#### Debug Output and Logging
```bash
# Enable Qt debug logging
QT_LOGGING_RULES="*.debug=true" ./pgpool-cpp

# Enable specific component logging
QT_LOGGING_RULES="qt.qpa.*=true" ./pgpool-cpp

# Verbose output
./pgpool-cpp --verbose

# Check Qt version and modules
./pgpool-cpp --version
```

### Database Connection Issues

#### PostgreSQL Service Status
```bash
# Check if PostgreSQL is running
sudo systemctl status postgresql

# Start PostgreSQL if stopped
sudo systemctl start postgresql

# Check PostgreSQL processes
ps aux | grep postgres

# Check listening ports
sudo netstat -tlnp | grep 5432
```

#### Connection Testing
```bash
# Test connection with psql
psql -h localhost -U your_username -d your_database

# Test with connection string
psql "postgresql://username:password@localhost:5432/database"

# Check PostgreSQL logs
sudo tail -f /var/log/postgresql/postgresql-*.log
```

#### Common Connection Errors
- **Connection refused**: PostgreSQL not running or wrong port
- **Authentication failed**: Incorrect username/password
- **Database does not exist**: Database name typo or database not created
- **Permission denied**: User lacks database access privileges

### Application Launch Issues

#### Missing Libraries
```bash
# Check for missing shared libraries
ldd ./pgpool-cpp

# Common missing libraries on Linux:
# - libQt6Core.so.6
# - libQt6Widgets.so.6
# - libQt6Network.so.6
# - libpqxx.so.6
# - libpq.so.5

# Install missing dependencies
sudo apt-get install libqt6core6 libqt6widgets6 libqt6network6
```

#### Permission Issues
```bash
# Make executable if needed
chmod +x ./pgpool-cpp

# Check file permissions
ls -la ./pgpool-cpp

# Run with sudo if needed (not recommended for GUI apps)
sudo ./pgpool-cpp
```

### Performance and Monitoring

#### Connection Pool Testing
```bash
# Monitor connection pool in real-time
# Use the "Test Pool" button in the application

# Check PostgreSQL connection count
SELECT count(*) FROM pg_stat_activity;

# Monitor active connections
SELECT pid, usename, application_name, state, query 
FROM pg_stat_activity 
WHERE state = 'active';
```

#### Memory and Resource Usage
```bash
# Monitor application memory usage
ps aux | grep pgpool-cpp

# Check for memory leaks
valgrind --tool=memcheck --leak-check=full ./pgpool-cpp

# Monitor system resources
htop
iotop
```

### Development and Debugging

#### Building with Debug Information
```bash
# Configure with debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build with debug symbols
ninja

# Run with GDB debugger
gdb ./pgpool-cpp

# Run with Valgrind for memory checking
valgrind --tool=memcheck --leak-check=full ./pgpool-cpp
```

#### Qt Debugging
```bash
# Enable Qt debug output
export QT_DEBUG_PLUGINS=1

# Check Qt plugin paths
./pgpool-cpp -platform xcb --verbose

# Monitor Qt events
QT_LOGGING_RULES="qt.events.*=true" ./pgpool-cpp
```

### Common Error Messages and Solutions

#### "Could not connect to display"
- **Solution**: Set `export DISPLAY=:0` or use X11 forwarding
- **Alternative**: Run with `-platform offscreen` for headless operation

#### "Qt platform plugin could not be initialized"
- **Solution**: Install missing Qt platform plugins
- **Alternative**: Force specific platform with `-platform xcb`

#### "Connection to database failed"
- **Solution**: Verify PostgreSQL is running and credentials are correct
- **Check**: Database name, username, password, host, and port

#### "Permission denied" errors
- **Solution**: Check file permissions and database user privileges
- **Alternative**: Run with appropriate user permissions

### Getting Help

#### Debug Information to Collect
When reporting issues, include:
1. **System information**: OS version, Qt version, compiler version
2. **Build output**: CMake configuration and build logs
3. **Runtime output**: Application logs and error messages
4. **Environment**: Display settings, Qt platform, environment variables
5. **Database status**: PostgreSQL version, connection test results

#### Useful Commands for Diagnosis
```bash
# System information
uname -a
cat /etc/os-release

# Qt information
qmake6 --version
qt6-config --version

# Compiler information
g++ --version
clang++ --version

# Library dependencies
ldd ./pgpool-cpp

# Environment variables
env | grep -E "(QT|DISPLAY|PATH)"

# Database connection test
psql -h localhost -U username -d database -c "SELECT version();"
```

## 📞 Support

For questions or issues related to this project, please refer to the code comments and implementation details in the source files.

**Contact**: tanner.davison95@gmail.com

## 🚀 Quick Start

1. **Clone the repository**
2. **Run setup script** (Linux): `./setup-qt.sh`
3. **Build the project**: `mkdir build && cd build && cmake .. && make`
4. **Launch the application**: `./pgpool-cpp`
5. **Configure your database connection**
6. **Start managing your PostgreSQL databases!**
