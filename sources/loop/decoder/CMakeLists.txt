set(OBJECTS
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

set(HEADERS
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
)

art_lib(artdecoder "${OBJECTS}" "${HEADERS}"  "artdecoder_linkdef.h" ".")