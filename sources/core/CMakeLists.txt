set(OBJECTS
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

set(HEADERS
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
)

art_lib(catcore "${OBJECTS}" "${HEADERS}"  "catcore_linkdef.h" "${HEADERS}")