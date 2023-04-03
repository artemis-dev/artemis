cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(artemis)

# ROOT 6 installation
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Default installation dir
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/ CACHE PATH "install dir" FORCE)

# Build options
option(BUILD_GET "Build and install libraries using GET" OFF)

# path definitions
if(BUILD_GET)
    set(WITH_GET_DECODER /opt/GETDecoder CACHE STRING "path to get decoder lib")
    set(GET_INCLUDE_DIR ${WITH_GET_DECODER}/include)
    set(GET_LIB_DIR ${WITH_GET_DECODER}/lib)
    set(GET_LIBRARIES GETDecoder)
endif()

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Physics Geom Minuit2)
find_package(MPI)
if(MPI_CXX_FOUND)
    add_compile_definitions(USE_MPI)
    add_compile_definitions(HAVE_MPI_H)
    include_directories(SYSTEM ${MPI_CXX_INCLUDE_PATH})
endif()

find_package(yaml-cpp)
if(yaml-cpp_FOUND)
    set(yaml_cpp_INCLUDEDIR ${YAML_CPP_INCLUDE_DIRS})
    set(yaml_cpp_LIBRARIES ${YAML_CPP_LIBRARIES})
    set(yaml_cpp_LIBDIR ${YAML_CPP_CMAKE_DIR}/../../../lib)
else()
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(yaml_cpp yaml-cpp)
    endif()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# link directories
link_directories(${yaml_cpp_LIBDIR})
if(BUILD_GET)
    link_directories(${GET_LIB_DIR})
endif()
link_directories(${ROOTSYS}/lib)

add_subdirectory(sources)
add_subdirectory(artemis-share)
add_subdirectory(cat-src)
