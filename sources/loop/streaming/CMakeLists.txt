set(LIB_NAME artstreaming)

add_library( ${LIB_NAME} SHARED
    TStreamingEventStore.cc
    TStreamingHeartBeatDelimiter.cc
    TStreamingSpillDelimiter.cc
    TStreamingModuleDecoder.cc
    TStreamingModuleDecoderFactory.cc
    TStreamingModuleDecoderLRTDC.cc
    TStreamingModuleDecoderHRTDC.cc
    TStreamingHeaderSTF.cc
    TStreamingHeaderFS.cc
    TStreamingHeaderTF.cc
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TStreamingSpillDelimiter.h
    TStreamingModuleDecoderLRTDC.h
    TStreamingModuleDecoderImpl.h
    TStreamingModuleDecoderHRTDC.h
    TStreamingModuleDecoder.h
    TStreamingModuleDecoderFactory.h
    TStreamingHeartBeatDelimiter.h
    TStreamingHeaderTF.h
    TStreamingHeaderSTF.h
    TStreamingHeaderFS.h
    TStreamingEventStore.h
    #TStreamingDataFileSinkComment.h    
    LINKDEF artstreaming_linkdef.h
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
