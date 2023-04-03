set(LIB_NAME catcore)

add_library( ${LIB_NAME} SHARED
    TDataObject.cc
    TConfigFile.cc
    TCatParameterTable.cc
    TCatPadManager.cc
    TCatHistManager.cc
    TArtemisUtil.cc
    TArtemisObject.cc
    TArtCanvas.cc
    TAnalysisInfo.cc
    TSharedMemory.cc
    TSemaphoreObject.cc
    TRunInfo.cc
    TProcessorParameter.cc
    TMacroReplacer.cc
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

if(MPI_CXX_FOUND)
        target_include_directories(${LIB_NAME} PRIVATE ${MPI_CXX_INCLUDE_PATH})
        target_link_libraries(${LIB_NAME} ${MPI_CXX_LIBRARIES})
endif()

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TConfigFile.h
    TCatParameterTable.h
    TCatPadManager.h
    TCatHistManager.h
    TArtemisUtil.h
    TArtemisObject.h
    TArtTypes.h
    TArtSystemOfUnit.h
    TArtCanvas.h
    TAnalysisInfo.h
    TSharedMemory.h
    TSemaphoreObject.h
    TRunInfo.h
    TProcessorParameter.h
    TMacroReplacer.h
    TDataObject.h    
    LINKDEF catcore_linkdef.h
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
