configure_file("modulefiles/artemis.in" artemis @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artemis DESTINATION share/modulefiles/)