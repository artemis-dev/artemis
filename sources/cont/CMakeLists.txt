set(LIB_NAME artcont)

add_library( ${LIB_NAME} SHARED
    TParameterObject.cc
    TCatReadoutPad.cc
    TCatReadoutPadArray.cc
    TCatPulseShape.cc
    TSimpleData.cc
    ICharge.cc
    ITiming.cc
    ITrack.cc
    IPosition.cc
    TDetectorGeometryInfo.cc
    TNArray.cc
    TTpcStaticGasProperty.cc
    TParameterStrings.cc   
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})
target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TTrack.h
    TTpcStaticGasProperty.h
    TTimingData.h
    #TTimingChargeData.h
    TTimeDifference.h
    TSimpleData.h
    TParameterStrings.h
    TParameterObject.h
    TNArray.h
    #TGenericData.h
    TDetectorGeometryInfo.h
    TCoinRegData.h
    TChargeData.h
    TCatScalerData.h
    TCatReadoutPad.h
    TCatReadoutPadArray.h
    TCatPulseShape.h
    ITrack.h
    ITiming.h
    IPosition.h
    ICharge.h    
    LINKDEF artcont_linkdef.h
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