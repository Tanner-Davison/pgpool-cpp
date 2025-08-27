cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
# Auto-detect platform and set vcpkg BEFORE project()
if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "F:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
elseif(APPLE)
    set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")  # Changed from x64-osx
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
else()
    set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()


# Now declare project
project(pgpool-cpp)

# Set C++ standard AFTER project()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Sql)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/InsertDialog.cpp  
    src/ConnectionPool.cpp
    src/TableCreator.cpp
    src/QueryExecutor.cpp
    src/DataModifier.cpp
    src/DatabaseManager.cpp
)

# Header files (for MOC processing)
set(HEADERS
    include/ConnectionPool.hpp
    include/DatabaseManager.hpp
    include/DataModifier.hpp
    include/DBOperation.hpp
    include/QueryExecutor.hpp
    include/TableCreator.hpp
    include/MainWindow.hpp
    include/InsertDialog.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        ${LIBPQXX_LIBRARIES}
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBPQXX_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBPQXX_CFLAGS_OTHER})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBPQXX_LIBRARY_DIRS}) 
# Qt deployment helpers for Linux
if(UNIX AND NOT APPLE)
    # Set RPATH for the executable
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
endif()

# Optional: Copy Qt plugins to output directory
if(Qt6_FOUND)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins"
        COMMENT "Creating plugins directory for Qt"
    )
endif()
