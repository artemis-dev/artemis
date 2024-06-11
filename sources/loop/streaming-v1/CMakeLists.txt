set(OBJECTS
  TStreamingEventStoreV1.cc
  TStreamingHeartBeatDelimiterV1.cc
  TStreamingModuleDecoderLRTDC64V1.cc
  TStreamingModuleDecoderHRTDCV1.cc
)

set(HEADERS
  TStreamingHeartBeatDelimiterV1.h
  TStreamingModuleDecoderLRTDC64V1.h
  TStreamingModuleDecoderHRTDCV1.h
  TStreamingHeaderTFV1.h
  TStreamingHeaderSTFV1.h
  TStreamingHeaderFSV1.h
  TStreamingEventStoreV1.h
  )
  
    

art_lib(artstreaming-v1 "${OBJECTS}" "${HEADERS}"  "artstreaming_linkdef.h" "${HEADERS}")
