cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
# Auto-detect platform and set vcpkg BEFORE project()
if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "F:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
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

find_package(libpqxx CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/ConnectionPool.cpp
    src/TableCreator.cpp
    src/QueryExecutor.cpp
    src/DataModifier.cpp
    src/DatabaseManager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)
