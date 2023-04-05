set(ARTEMIS_INCLUDE_DIRS
    ${ARTEMIS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/sources/core
    ${CMAKE_SOURCE_DIR}/sources/cont
    ${CMAKE_SOURCE_DIR}/sources/commands
    ${CMAKE_SOURCE_DIR}/sources/loop
    ${CMAKE_SOURCE_DIR}/sources/loop/decoder
    ${CMAKE_SOURCE_DIR}/sources/loop/hist
    ${CMAKE_SOURCE_DIR}/sources/loop/get
    ${CMAKE_SOURCE_DIR}/sources/loop/param
    ${CMAKE_SOURCE_DIR}/sources/loop/rdf
    ${CMAKE_SOURCE_DIR}/sources/loop/ridf
    ${CMAKE_SOURCE_DIR}/sources/loop/streaming
    ${CMAKE_SOURCE_DIR}/sources/mc
    ${CMAKE_SOURCE_DIR}/sources/main
    CACHE INTERNAL ""
)

add_subdirectory(core)
add_subdirectory(cont)
add_subdirectory(commands)
add_subdirectory(mc)
add_subdirectory(loop)
add_subdirectory(main)
