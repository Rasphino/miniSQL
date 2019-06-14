cmake_minimum_required(VERSION 3.10)
project(DB_miniSQL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (WIN32)
    message(STATUS ">>> Win")
    add_definitions(-DWIN32)
endif ()

if (UNIX)
    message(STATUS ">>> UNIX")
    add_definitions(-DUNIX)
endif ()

find_package(RapidJSON CONFIG REQUIRED)

add_executable(DB_miniSQL src/main.cpp src/GlobalData.h src/BufferManager.cpp src/BufferManager.h src/CatalogManager.cpp src/CatalogManager.h)

target_include_directories(DB_miniSQL PRIVATE ${RAPIDJSON_INCLUDE_DIRS})
