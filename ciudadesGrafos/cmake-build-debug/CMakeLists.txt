cmake_minimum_required(VERSION 3.24)
project(ciudadesGrafos C)

set(CMAKE_C_STANDARD 11)

include_directories(..)

add_executable(ETSEMaps ../grafo.c ../main.c ../ETSEMaps.c)
