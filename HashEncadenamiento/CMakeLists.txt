cmake_minimum_required(VERSION 3.24)
project(HashEncadenamiento C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(HashEncadenamiento lista.c main.c tabla_hash_encadenamiento.c)
