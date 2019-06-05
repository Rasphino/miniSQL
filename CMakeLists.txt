cmake_minimum_required(VERSION 3.14)
project(DB_miniSQL)

set(CMAKE_CXX_STANDARD 14)

add_executable(DB_miniSQL src/main.cpp src/GlobalData.h src/BufferManager.cpp src/BufferManager.h src/CatalogManager.cpp src/CatalogManager.h)