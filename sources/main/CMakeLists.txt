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

configure_file("artemis-config.in" artemis-config @ONLY)
configure_file("thisartemis.sh.in" thisartemis.sh @ONLY)
configure_file("thisartemis.csh.in" thisartemis.csh @ONLY)

file(
        COPY ${CMAKE_CURRENT_BINARY_DIR}/artemis-config
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

file(
        COPY ${CMAKE_CURRENT_BINARY_DIR}/thisartemis.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

file(
        COPY ${CMAKE_CURRENT_BINARY_DIR}/thisartemis.csh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

