cmake_minimum_required(VERSION 3.13)

project(tracer
    LANGUAGES CXX 
    VERSION 0.17
)


message(STATUS "Enter the library folder")

#generate static library
add_library(${PROJECT_NAME} STATIC)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_NAME}.cpp
)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#This module contains the information about
#where to install the static library
include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    #"ARCHIVE" means the static library
    ARCHIVE
    	DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    	COMPONENT "core"
)

#install the header
install(
    FILES "${PROJECT_NAME}.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
set(CPACK_PACKAGE_DESCRIPTION
    "This is a simple ray-tracing library that is intended for analysing the optical lens system using geometric optics."
)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Simple c++ geometric rays analysis library"
)
#generator for generating a ".deb" package
set(CPACK_GENERATOR "DEB")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "wang (cuboidtrick, warren, etc)")
set(CPACK_PACKAGE_CONTACT "warrenleywon@gmail.com")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})


set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
include(CPack)
