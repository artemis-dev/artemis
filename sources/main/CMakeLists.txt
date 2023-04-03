add_executable(artemis main.cc TArtRint.cc)
target_include_directories(artemis PRIVATE ${ARTEMIS_INCLUDE_DIRS})
target_link_libraries(artemis ${ROOT_LIBRARIES} ${yaml_cpp_LIBRARIES} ${ARTEMIS_LIBRARIES})
if(MPI_CXX_FOUND)
        target_include_directories(artemis PRIVATE ${MPI_CXX_INCLUDE_PATH})
        target_link_libraries(artemis ${MPI_CXX_LIBRARIES})
endif()

install(TARGETS artemis
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
