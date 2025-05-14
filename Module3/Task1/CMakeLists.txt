cmake_minimum_required(VERSION 3.10)

project(GraphRealizeProject)

include_directories(include)

add_executable(a.out
        src/main.cpp
        src/ArcGraph.cpp
        src/ListGraph.cpp
        src/MatrixGraph.cpp
        src/SetGraph.cpp
    )