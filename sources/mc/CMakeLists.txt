set(LIB_NAME catmc)

add_library( ${LIB_NAME} SHARED
	TArtEventCollections.cc
	TArtParticle.cc
	TArtAtomicMassTable.cc
	TCatTwoBodyKinematics.cc
	TBinaryReactionGenerator.cc
	TTrackG4.cc
	TRandomGraph.cc 
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TTrackG4.h
    TRandomGraph.h
    TCatTwoBodyKinematics.h
    TBinaryReactionGenerator.h
    TArtParticle.h
    TArtEventCollections.h
    TArtAtomicMassTable.h    
    LINKDEF catmc_linkdef.h
    MODULE ${LIB_NAME}
)

install(TARGETS ${LIB_NAME}
    LIBRARY DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${LIB_NAME}_rdict.pcm
    DESTINATION lib
)
