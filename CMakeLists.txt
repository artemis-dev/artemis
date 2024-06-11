cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(artemis)

# ROOT 6 installation
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Default installation dir
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/ CACHE PATH "install dir")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Build options
option(BUILD_GET "Build and install libraries using GET" OFF)

# Locate GET decoder if the option is on
if(BUILD_GET)
  set(WITH_GET_DECODER /opt/GETDecoder CACHE STRING "path to get decoder lib")
  set(GET_INCLUDE_DIR ${WITH_GET_DECODER}/include)
  set(GET_LIB_DIR ${WITH_GET_DECODER}/lib)
  set(GET_LIBRARIES GETDecoder)
endif()

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Physics Geom Minuit Minuit2 Gui)
find_package(MPI)
if(MPI_CXX_FOUND)
  add_compile_definitions(USE_MPI)
  add_compile_definitions(HAVE_MPI_H)
  include_directories(SYSTEM ${MPI_CXX_INCLUDE_PATH})
endif()

find_package(ZeroMQ)
if(ZeroMQ_FOUND)
  add_compile_definitions(HAVE_ZMQ_H)
  include_directories(${ZeroMQ_INCLUDE_DIR})
endif()

pkg_check_modules(REDIS redis++)
pkg_check_modules(HIREDIS hiredis)
if (REDIS_FOUND AND HIREDIS_FOUND)
  add_compile_definitions(HAVE_REDIS_H)
  include_directories(${REDIS_INCLUDE_DIR})
  message(STATUS "redis++ and hiredis is found")
  message(STATUS ${REDIS_LIBRARY_DIRS})
  message(STATUS ${HIREDIS_LIBRARY_DIRS})
endif()

# Locate the yaml-cpp package
# If find_package(yaml-cpp) faild, it tries pkg_check_modules()
find_package(yaml-cpp)
if(yaml-cpp_FOUND)
    set(yaml_cpp_INCLUDEDIR ${YAML_CPP_INCLUDE_DIR})
    set(yaml_cpp_LIBRARIES ${YAML_CPP_LIBRARIES})
    set(yaml_cpp_LIBDIR ${YAML_CPP_LIBRARY_DIR})
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(yaml_cpp REQUIRED yaml-cpp)
endif()

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# external link directories
link_directories(${yaml_cpp_LIBDIR})
if(BUILD_GET)
    link_directories(${GET_LIB_DIR})
endif()
link_directories(${ROOTSYS}/lib)

# Internal cache variables
set(ARTEMIS_INCLUDE_DIRS "" CACHE INTERNAL "")
set(ARTEMIS_LIBRARIES "" CACHE INTERNAL "")
set(ARTEMIS_HEADER_DESTINATIONS "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-Wall -g")

# cmake function for building an artemis library
# lib_name: Library name
# objects: list of .cc files
# headers: list of .h files
# linkdef_name: name of linkdef.h for ROOT dictionary generation
# header_destination: name of the directory containing the header files
function(art_lib lib_name objects dic_headers linkdef_name pub_headers)

    add_library(${lib_name} SHARED ${objects})

    target_include_directories(${lib_name}
        PUBLIC
            $<INSTALL_INTERFACE:include/${header_destination}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    target_include_directories(${lib_name}
	    PRIVATE
            ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS}
    )
    target_link_libraries(${lib_name}
        ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES} ${ARTEMIS_LIBRARIES} 
    )
    if(MPI_CXX_FOUND)
            target_include_directories(${lib_name} PRIVATE ${MPI_CXX_INCLUDE_PATH})
            target_link_libraries(${lib_name} ${MPI_CXX_LIBRARIES})
    endif()

    if(BUILD_GET)
      target_include_directories(${lib_name} PRIVATE ${GET_INCLUDE_DIR} ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})
      target_link_libraries(${lib_name} ${GET_LIBRARIES})
    endif()
    
    if (ZeroMQ_FOUND)
      target_include_directories(${lib_name} PRIVATE ${ZeroMQ_INCLUDE_DIR})
      target_link_libraries(${lib_name} ${ZeroMQ_LIBRARY})
    endif()

    if (REDIS_FOUND)
      target_include_directories(${lib_name} PRIVATE ${REDIS_INCLUDE_DIR})
      target_link_directories(${lib_name} PRIVATE ${REDIS_LIBRARY_DIRS})
      target_link_directories(${lib_name} PRIVATE ${HIREDIS_LIBRARY_DIRS})
      target_link_libraries(${lib_name} ${REDIS_LIBRARIES})
    endif()
      
        
    ROOT_GENERATE_DICTIONARY(G__${lib_name}
        ${dic_headers}
        LINKDEF ${linkdef_name}
        MODULE ${lib_name}
    )

    set_target_properties(
        ${lib_name}
        PROPERTIES
            PUBLIC_HEADER "${pub_headers}"    
    )

    install(TARGETS ${lib_name}
        EXPORT artemisTargets 
        INCLUDES DESTINATION include
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${header_destination}
        LIBRARY DESTINATION lib
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${lib_name}_rdict.pcm
        DESTINATION lib 
    )

    set(ARTEMIS_LIBRARIES ${ARTEMIS_LIBRARIES} ${lib_name} CACHE INTERNAL "")
    set(ARTEMIS_HEADER_DESTINATIONS ${ARTEMIS_HEADER_DESTINATIONS} ${header_destination} CACHE INTERNAL "")
endfunction()

add_subdirectory(sources)
add_subdirectory(artemis-share)
add_subdirectory(cat-src)
add_subdirectory(share)

install(EXPORT artemisTargets
    FILE artemisTargets.cmake
    NAMESPACE artemis::
    DESTINATION lib/cmake/artemis
)


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/artemis-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/artemis-config.cmake"
    @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/artemis-config.cmake"
    DESTINATION lib/cmake/artemis
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "artemis-config.cmake.in"
    "artemis-config.cmake"
    INSTALL_DESTINATION lib/cmake/artemis
)

# Install artemis-config.cmake so find_package(artemis) works
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/artemis-config.cmake"
    DESTINATION lib/cmake/artemis
)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
