set(OBJECTS
    TStreamingEventStoreV1.cc
    TStreamingHeartBeatDelimiterV1.cc
    TStreamingModuleDecoderLRTDC64V1.cc
    TStreamingModuleDecoderHRTDCV1.cc
#    TStreamingHeaderSTFV1.cc
#    TStreamingHeaderFSV1.cc
#    TStreamingHeaderTFV1.cc
#    TStreamingHeaderFLTCOIN.cc
)

set(HEADERS
    TStreamingHeartBeatDelimiterV1.h
    TStreamingModuleDecoderLRTDC64V1.h
    TStreamingModuleDecoderHRTDCV1.h
    TStreamingHeaderTFV1.h
    TStreamingHeaderSTFV1.h
    TStreamingHeaderFSV1.h
#    TStreamingHeaderFLTCOIN.h
    TStreamingEventStoreV1.h
    #TStreamingDataFileSinkComment.h    
)

art_lib(artstreaming-v1 "${OBJECTS}" "${HEADERS}"  "artstreaming_linkdef.h" "${HEADERS}")
