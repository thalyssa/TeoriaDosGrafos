cmake_minimum_required(VERSION 3.20)
project(ocdt_graph_theory)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/ford-fulkerson)
add_executable(ford-fulkerson src/ford-fulkerson/fulkerson.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/dijkstra)
add_executable(dijkstra src/dijkstra/dijkstra.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/kruskal)
add_executable(kruskal src/kruskal/kruskal.cpp)

