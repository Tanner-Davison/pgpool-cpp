# C++ PostgreSQL Project

A simple C++ application that connects to PostgreSQL using libpqxx.

## Prerequisites

- MinGW-w64 (via MSYS2)
- PostgreSQL 15
- libpqxx (installed via vcpkg)
- CMake

## Project Structure
```
cpp-postgress/
├── src/
│   └── main.cpp
├── build/          (generated)
├── CMakeLists.txt
├── .clangd         (LSP config)
├── .ignore         (performance)
└── README.md
```

## Building the Project

```bash
# Navigate to project directory
cd F:\Projects\cpp\cpp-postgress

# Create and enter build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build the project
cmake --build .

# Copy compile_commands.json for Neovim LSP
copy compile_commands.json ..
```

## Running the Application

```bash
# From build directory
.\cpp-postgress.exe
```

## PostgreSQL Connection

The app connects using these credentials:

- Database: postgres
- User: postgres
- Password: [your password]
- Host: localhost
- Port: 5432 (default)

## Managing PostgreSQL Service

```powershell
# Check if PostgreSQL is running
Get-Service postgresql-x64-15

# Start PostgreSQL
Start-Service postgresql-x64-15

# Stop PostgreSQL
Stop-Service postgresql-x64-15
```

## Troubleshooting

### Connection Failed

- Ensure PostgreSQL service is running
- Check password in src/main.cpp
- Verify PostgreSQL is listening on localhost:5432

### Slow Neovim/LSP

- Delete .cache/ folder if it exists
- Ensure .clangd config is present
- Restart LSP with :LspRestart

### Rebuild from Clean State

```bash
cd build
Remove-Item * -Recurse -Force
cmake .. -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build .
```

## Quick Development Workflow

```bash
# 1. Edit code
nvim src/main.cpp

# 2. Build
cd build && cmake --build .

# 3. Run
.\cpp-postgress.exe
```

## Dependencies Info

- **libpqxx**: Installed at F:/tools/vcpkg/installed/x64-mingw-dynamic/
- **Compiler**: MinGW-w64 GCC 14.2.0 from MSYS2
- **Build System**: CMake with MinGW Makefiles
# cpp-postgress
