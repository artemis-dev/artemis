set(OBJECTS
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

set(HEADERS
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
)

art_lib(artstreaming "${OBJECTS}" "${HEADERS}"  "artstreaming_linkdef.h" ".")
