#!/bin/bash

# Qt6 Setup Script for Linux (Ubuntu/Debian based)
# Run with: chmod +x setup-qt.sh && ./setup-qt.sh

echo "=== Qt6 Setup for pgpool-cpp project ==="
echo ""

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Update package list
echo "Updating package list..."
sudo apt update

# Install Qt6 dependencies
echo "Installing Qt6 and dependencies..."
sudo apt install -y \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6network6 \
    libqt6sql6 \
    libqt6sql6-psql \
    libgl1-mesa-dev \
    build-essential \
    cmake \
    ninja-build

# Install PostgreSQL client libraries if not present
if ! pkg-config --exists libpq; then
    echo "Installing PostgreSQL client libraries..."
    sudo apt install -y libpq-dev postgresql-client
fi

# Install vcpkg if not present
if [ ! -d "$HOME/tools/vcpkg" ]; then
    echo "Installing vcpkg..."
    mkdir -p $HOME/tools
    cd $HOME/tools
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    
    # Install libpqxx via vcpkg
    echo "Installing libpqxx via vcpkg..."
    ./vcpkg install libpqxx
fi

# Create build directory structure
echo "Setting up project build structure..."
cd -  # Return to original directory

if [ ! -d "build" ]; then
    mkdir build
fi

if [ ! -d "include" ]; then
    mkdir include
fi

# Generate compile_commands.json for IDE support
echo "Configuring project with CMake..."
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Create a sample .env file for database configuration
cd ..
if [ ! -f ".env" ]; then
    echo "Creating sample .env file..."
    cat > .env << 'EOF'
# Database Configuration
DB_HOST=localhost
DB_PORT=5432
DB_NAME=testdb
DB_USER=postgres
DB_PASSWORD=your_password
POOL_SIZE=5
EOF
fi

# QtCreator project file for easier IDE integration
cat > pgpool-cpp.pro << 'EOF'
TEMPLATE = app
TARGET = pgpool-cpp
QT += core gui widgets network sql
CONFIG += c++17

INCLUDEPATH += include/

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/InsertDialog.cpp \
    src/ConnectionPool.cpp \
    src/DatabaseManager.cpp \
    src/DataModifier.cpp \
    src/QueryExecutor.cpp \
    src/TableCreator.cpp

HEADERS += \
    include/MainWindow.hpp \
    include/ConnectionPool.hpp \
    include/DatabaseManager.hpp \
    include/DataModifier.hpp \
    include/DBOperation.hpp \
    include/QueryExecutor.hpp \
    include/TableCreator.hpp \
    include/InsertDialog.hpp 

LIBS += -lpqxx -lpq
EOF

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Next steps:"
echo "1. Place MainWindow.hpp in the include/ directory"
echo "2. Place MainWindow.cpp in the src/ directory"
echo "3. Update src/main.cpp with the Qt version"
echo "4. Build the project:"
echo "   cd build"
echo "   cmake .."
echo "   make -j$(nproc)"
echo ""
echo "To run the application:"
echo "   ./build/pgpool-cpp"
echo ""
echo "For Qt Creator:"
echo "   Open pgpool-cpp.pro in Qt Creator"
echo ""
