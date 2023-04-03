set(LIB_NAME artdecoder)

add_library( ${LIB_NAME} SHARED
	TModuleDecoderSIS3820.cc
	TModuleDecoderSIS3610.cc
	TModuleDecoderV1190.cc
	TModuleDecoderV1740.cc
	TModuleDecoderSkip.cc
	TModuleDecoderV767.cc
	TModuleDecoderV7XX.cc
	TModuleDecoderFixed.cc
	TModuleDecoderV1290.cc
	TModuleDecoderMXDC32.cc
	TModuleDecoderV550.cc
	TRawDataFadc.cc
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    #TRawDataV767.h
    TRawDataFadc.h
    TModuleDecoderV7XX.h
    TModuleDecoderV767.h
    TModuleDecoderV550.h
    TModuleDecoderV1740.h
    TModuleDecoderV1290.h
    TModuleDecoderV1190.h
    TModuleDecoderSkip.h
    TModuleDecoderSIS3820.h
    TModuleDecoderSIS3610.h
    TModuleDecoderMXDC32.h
    TModuleDecoderFixed.h
    LINKDEF artdecoder_linkdef.h
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
