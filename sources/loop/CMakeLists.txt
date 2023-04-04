add_subdirectory(decoder)
if(BUILD_GET)
    add_subdirectory(get)
endif()
add_subdirectory(hist)
add_subdirectory(param)
add_subdirectory(rdf)
add_subdirectory(ridf)
add_subdirectory(streaming)

set(LOOPLIB_NAME catloop)

add_library( ${LOOPLIB_NAME} SHARED
    TCategorizedData.cc 
    TConditionBit.cc 
    TCounterEventStore.cc 
    TEventCollection.cc 
    TEventHeader.cc
    TEventObject.cc 
    TFileDataSource.cc 
    TFileDataSourceGZ.cc 
    TLoop.cc 
    TLoopControl.cc 
    TLoopManager.cc 
    TMapTable.cc 
    TMappingProcessor.cc
    TModuleDecoder.cc 
    TModuleDecoderFactory.cc
    TModuleInfo.cc
    TModuleType.cc
    TOutputTreeProcessor.cc 
    TProcessor.cc 
    TRandomNumberEventStore.cc 
    TRandomNumberProcessor.cc 
    TRawDataObject.cc 
    TRawDataSimple.cc 
    TRawDataTimingCharge.cc 
    TRawTiming.cc 
    TRawTimingWithEdge.cc 
    TSegmentCheckProcessor.cc
    TSegmentedData.cc
    TSegmentInfo.cc
    TSharedMemoryDataSource.cc
    TTimerProcessor.cc 
    TTreeEventStore.cc 
    IProcessorHelper.cc
    TScalerData.cc
    TTimestampEventList.cc
    TTimestampEventReconstructor.cc
    TScalerTimestampProcessor.cc
    THeaderTimestampProcessor.cc
    TScalerMappingProcessor.cc
    TObjectLoader.cc
    TAsynchronousRunEndProcessor.cc
    TPulseShapeTimestampProcessor.cc
    TTimeValidator.cc
    TChargeValidator.cc
    TTimingTimestampAdapter.cc
    TTimestampValidator.cc
    TGainShiftCorrectionProcessor.cc
    IEventStore.cc
    TFormulaData.cc
    TFormulaDataGenerator.cc
    TEventNumberConditionProcessor.cc
    TArtTree.cc
    TRawDataMappingProcessor.cc
    TAttParameter.cc
)

target_include_directories(${LOOPLIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})

target_link_libraries(${LOOPLIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES} catcore artcont)

if(MPI_CXX_FOUND)
        target_include_directories(${LOOPLIB_NAME} PRIVATE ${MPI_CXX_INCLUDE_PATH})
        target_link_libraries(${LOOPLIB_NAME} ${MPI_CXX_LIBRARIES})
endif()

ROOT_GENERATE_DICTIONARY(G__${LOOPLIB_NAME}
    TTreeEventStore.h
    TTimingTimestampAdapter.h
    TTimeValidator.h
    TTimestampValidator.h
    TTimestampEventReconstructor.h
    TTimestampEventList.h
    TTimerProcessor.h
    TSharedMemoryDataSource.h
    TSegmentInfo.h
    TSegmentedData.h
    TSegmentedDataCheckProcessor.h
    TSegmentCheckProcessor.h
    TScalerTimestampProcessor.h
    TScalerMappingProcessor.h
    TScalerData.h
    TRawTimingWithEdge.h
    TRawTiming.h
    TRawDataTimingCharge.h
    TRawDataSimple.h
    TRawDataObject.h
    TRawDataMappingProcessor.h
    TRandomNumberProcessor.h
    TRandomNumberEventStore.h
    TPulseShapeTimestampProcessor.h
    TProcessor.h
    TOutputTreeProcessor.h
    TObjectLoader.h
    TModuleType.h
    TModuleInfo.h
    TModuleDecoder.h
    TModuleDecoderFactory.h
    TMapTable.h
    TMappingProcessor.h
    TLoopManager.h
    TLoop.h
    TLoopControl.h
    THeaderTimestampProcessor.h
    TGainShiftCorrectionProcessor.h
    TFormulaData.h
    TFormulaDataGenerator.h
    TFileDataSource.h
    TFileDataSourceGZ.h
    TEventObject.h
    TEventNumberConditionProcessor.h
    TEventHeader.h
    TEventCollection.h
    TDataSource.h
    TCounterEventStore.h
    TConditionBit.h
    TChargeValidator.h
    #TCatHistProcessor.h
    TCategorizedData.h
    TAttParameter.h
    TAsynchronousRunEndProcessor.h
    TArtTree.h
    IProcessorHelper.h
    IEventStore.h
    LINKDEF catloop_linkdef.h
    MODULE ${LOOPLIB_NAME}
)

install(TARGETS ${LOOPLIB_NAME}
    LIBRARY DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${LOOPLIB_NAME}_rdict.pcm
    DESTINATION lib
)