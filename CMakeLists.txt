cmake_minimum_required(VERSION 3.20)
# Change to Linux triplet
# Auto-detect platform
if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "F:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
else()
    set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()
project(cpp-postgress)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(libpqxx CONFIG REQUIRED)
set(SOURCES
    src/main.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)
