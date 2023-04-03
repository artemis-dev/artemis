set(LIB_NAME arthist)

add_library( ${LIB_NAME} SHARED
    TAttCut.cc
    TAttTreeProj.cc
    TAxisTreeProj.cc
    TH1FTreeProj.cc
    TH2FTreeProj.cc
    TH3FTreeProj.cc
    TTreeProjection.cc
    TTreeProjectionProcessor.cc
    TTreeProjGroup.cc
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

if(MPI_CXX_FOUND)
        target_include_directories(${LIB_NAME} PRIVATE ${MPI_CXX_INCLUDE_PATH})
        target_link_libraries(${LIB_NAME} ${MPI_CXX_LIBRARIES})
endif()

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TTreeProjGroup.h
    TTreeProjectionProcessor.h
    TTreeProjection.h
    TH3FTreeProj.h
    TH2FTreeProj.h
    TH1FTreeProj.h
    TAxisTreeProj.h
    TAttTreeProj.h
    TAttCut.h
    LINKDEF arthist_linkdef.h
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
