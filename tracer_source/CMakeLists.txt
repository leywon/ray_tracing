cmake_minimum_required(VERSION 3.13)

project(phys_raytrace
    LANGUAGES CXX
    VERSION 0.17
    DESCRIPTION "Simple ray tracing library for geometric optics"
)

#execute the cmake script in the subdirectory
add_subdirectory("src")