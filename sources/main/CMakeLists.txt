add_executable(artemis main.cc TArtRint.cc)
target_include_directories(artemis PRIVATE ${ARTEMIS_INCLUDE_DIRS})
target_link_libraries(artemis ${ROOT_LIBRARIES} ${yaml_cpp_LIBRARIES} ${ARTEMIS_LIBRARIES})
if(MPI_CXX_FOUND)
        target_include_directories(artemis PRIVATE ${MPI_CXX_INCLUDE_PATH})
        target_link_libraries(artemis ${MPI_CXX_LIBRARIES})
endif()

set_target_properties(
        artemis
        PROPERTIES
                PUBLIC_HEADER "TArtRint.h"        
)

install(TARGETS artemis
        EXPORT artemisTargets
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        PUBLIC_HEADER DESTINATION include/main
        LIBRARY DESTINATION lib
)

configure_file("artemis-config.in" artemis-config @ONLY)
configure_file("thisartemis.sh.in" thisartemis.sh @ONLY)
configure_file("thisartemis.csh.in" thisartemis.csh @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artemis-config
	DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
	
install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/thisartemis.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/thisartemis.csh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

