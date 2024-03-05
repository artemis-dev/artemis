set(OBJECTS
    TStreamingEventStore.cc
    TStreamingHeartBeatDelimiter.cc
    TStreamingSpillDelimiter.cc
    TStreamingModuleDecoder.cc
    TStreamingModuleDecoderFactory.cc
    TStreamingModuleDecoderLRTDC.cc
    TStreamingModuleDecoderHRTDC.cc
    TStreamingModuleDecoderLRTDC64.cc
    TStreamingHeaderSTF.cc
    TStreamingHeaderFS.cc
    TStreamingHeaderTF.cc
    TStreamingHeaderFLTCOIN.cc
)

set(HEADERS
    TStreamingSpillDelimiter.h
    TStreamingModuleDecoderLRTDC.h
    TStreamingModuleDecoderImpl.h
    TStreamingModuleDecoderHRTDC.h
    TStreamingModuleDecoderLRTDC64.h
    TStreamingModuleDecoder.h
    TStreamingModuleDecoderFactory.h
    TStreamingHeartBeatDelimiter.h
    TStreamingHeaderTF.h
    TStreamingHeaderSTF.h
    TStreamingHeaderFS.h
    TStreamingHeaderFLTCOIN.h
    TStreamingEventStore.h
    #TStreamingDataFileSinkComment.h    
)

art_lib(artstreaming "${OBJECTS}" "${HEADERS}"  "artstreaming_linkdef.h" "${HEADERS}")
