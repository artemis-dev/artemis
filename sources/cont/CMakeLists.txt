set(OBJECTS
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

set(HEADERS
    TTpcStaticGasProperty.h
    TSimpleData.h
    TParameterStrings.h
    TParameterObject.h
    TNArray.h
    TDetectorGeometryInfo.h
    TCatReadoutPad.h
    TCatReadoutPadArray.h
    TCatPulseShape.h
    ITrack.h
    ITiming.h
    IPosition.h
    ICharge.h
)

art_lib(artcont "${OBJECTS}" "${HEADERS}"  "artcont_linkdef.h" ".")