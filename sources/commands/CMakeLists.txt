set(LIB_NAME catcmd)


add_library( ${LIB_NAME} SHARED
	TCatCmdFactory.cc
	TCatCmd.cc
	TCatCmdHelp.cc
	TCatCmdHt.cc
	TCatCmdHtp.cc
	TCatCmdHb.cc
	TCatCmdHn.cc
	TCatCmdZone.cc
	TCatCmdPr.cc
	TCatCmdPrx.cc
	TCatCmdPry.cc
	TCatCmdBn.cc
	TCatCmdBnx.cc
	TCatCmdBny.cc
	TCatCmdAv.cc
	TCatCmdAvx.cc
	TCatCmdAvy.cc
	TCatCmdMn.cc
	TCatCmdMnx.cc
	TCatCmdMny.cc
	TCatCmdSl.cc
	TCatCmdSly.cc
	TCatCmdLs.cc
	TCatCmdCd.cc
	TCatCmdListg.cc
	TCatCmdHstore.cc
	TCatCmdSave.cc
	TCatCmdMacro.cc
	TCatCmdLg.cc
	TCatCmdXval.cc
	TCatCmdLoopAdd.cc
	TCatCmdLoopResume.cc
	TCatCmdLoopSuspend.cc
	TCatCmdLoopTerminate.cc
	TCmdUpdateCanvas.cc
	TCmdBranchInfo.cc
	TCmdClassInfo.cc
	TCmdHdel.cc
	TCmdGroup.cc
	TCmdFileCd.cc
	TCmdFileLs.cc
	TCmdFipo.cc
	TCmdUnZoom.cc
	TCmdRg.cc
	TCmdPcd.cc
	TCmdPb.cc
	TCmdPn.cc
	TCmdComment.cc
	TCmdMpol.cc
	TCmdSlope.cc
	TCmdSave.cc
	TCmdPrint.cc
	TCmdPadZoom.cc
	TCmdHcol.cc
	TCmdProcessorDescription.cc
	TCmdMergeFile.cc
	TCmdGlobalComment.cc
	TCmdUpload.cc
)

target_include_directories(${LIB_NAME}
    PUBLIC ${yaml_cpp_INCLUDEDIR} ${ARTEMIS_INCLUDE_DIRS})
target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES} catcore)

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
    TCmdUpload.h
    TCmdUpdateCanvas.h
    TCmdUnZoom.h
    TCmdSlope.h
    TCmdSave.h
    TCmdRg.h
    TCmdProcessorDescription.h
    TCmdPrint.h
    TCmdPn.h
    TCmdPcd.h
    TCmdPb.h
    TCmdPadZoom.h
    TCmdMpol.h
    TCmdMergeFile.h
    TCmdHdel.h
    TCmdHcol.h
    TCmdGroup.h
    TCmdGlobalComment.h
    TCmdFipo.h
    TCmdFileLs.h
    TCmdFileCd.h
    TCmdComment.h
    TCmdClassInfo.h
    TCmdBranchInfo.h
    TCatCmdZone.h
    TCatCmdXval.h
    TCatCmdSly.h
    TCatCmdSl.h
    TCatCmdSave.h
    TCatCmdPry.h
    TCatCmdPrx.h
    TCatCmdPr.h
    TCatCmdMny.h
    TCatCmdMnx.h
    TCatCmdMn.h
    TCatCmdMacro.h
    TCatCmdLs.h
    TCatCmdLoopTerminate.h
    TCatCmdLoopSuspend.h
    TCatCmdLoopResume.h
    TCatCmdLoopAdd.h
    TCatCmdListg.h
    TCatCmdLg.h
    TCatCmdHtp.h
    TCatCmdHt.h
    TCatCmdHstore.h
    TCatCmdHn.h
    TCatCmdHelp.h
    TCatCmdHb.h
    TCatCmd.h
    TCatCmdFactory.h
    TCatCmdCd.h
    TCatCmdBny.h
    TCatCmdBnx.h
    TCatCmdBn.h
    TCatCmdAvy.h
    TCatCmdAvx.h
    TCatCmdAv.h    
    LINKDEF catcmd_linkdef.h
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
