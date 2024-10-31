cmake_minimum_required(VERSION 3.24)
project(HashRecolocacion C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(HashRecolocacion main.c tabla_hash_recolocacion.c)
