/* $Id:$ */
/**
 * @file   catloop_linkdef.h
 * @date   Created : Feb 19, 2012 11:19:06 JST
 *   Last Modified : 2018-07-18 14:12:01 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef CATLOOP_LINKDEF_H
#define CATLOOP_LINKDEF_H

#ifdef __CINT__
#define _SYS_TYPES_H_


#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// #pragma link C++ class TCatEventCollectionManager-!;

#pragma link C++ class art::TLoop;
#pragma link C++ class art::TLoopManager;
#pragma link C++ class art::TConditionBit;
#pragma link C++ class art::TEventObject;
#pragma link C++ class art::TEventCollection;
#pragma link C++ class art::TProcessor;
#pragma link C++ namespace art;
#pragma link C++ global art::gProcessors;
#pragma link C++ class art::TTimerProcessor;
#pragma link C++ class art::TOutputTreeProcessor;
#pragma link C++ class art::TCounterEventStore;
#pragma link C++ class art::TRandomNumberEventStore;
#pragma link C++ class art::TRandomNumberProcessor;
#pragma link C++ class art::TTreeEventStore;
#pragma link C++ class art::TRawDataObject;
#pragma link C++ class art::TSegmentedData;
#pragma link C++ class art::TCategorizedData;
#pragma link C++ class art::TMapTable;
//#pragma link C++ class art::TFileDataSource-;
//#pragma link C++ class art::TFileDataSourceGZ;
#pragma link C++ class art::TModuleDecoder;
#pragma link C++ class art::TModuleDecoderFactory;
#pragma link C++ class art::TRawDataSimple<UChar_t>;
#pragma link C++ class art::TRawDataSimple<UShort_t>;
#pragma link C++ class art::TRawDataSimple<UInt_t>;
#pragma link C++ class art::TRawDataSimple<ULong64_t>;
#pragma link C++ class art::TRawTiming;
#pragma link C++ class art::TRawTimingWithEdge;
#pragma link C++ class art::TEventHeader;
#pragma link C++ class art::TMappingProcessor;
//#pragma link C++ class art::TSharedMemoryDataSource;
#pragma link C++ class art::TSegmentInfo;
#pragma link C++ class art::TSegmentCheckProcessor;
#pragma link C++ class art::TModuleInfo;
#pragma link C++ class art::TModuleType;
#pragma link C++ class art::IProcessorHelper;
#pragma link C++ class art::TScalerData+;
#pragma link C++ class art::TTimestampEventList+;
#pragma link C++ class art::TTimestampEventReconstructor;
#pragma link C++ class art::TScalerTimestampProcessor;
#pragma link C++ class art::THeaderTimestampProcessor;
#pragma link C++ class art::TScalerMappingProcessor;
#pragma link C++ class art::TObjectLoader;
#pragma link C++ class art::TAsynchronousRunEndProcessor;
#pragma link C++ class art::TPulseShapeTimestampProcessor;
#pragma link C++ class art::TTimeValidator;
#pragma link C++ class art::TTimingTimestampAdapter;
#pragma link C++ class art::TTimestampValidator;
#endif /* __CINT__ */

#endif // end of #ifdef CATLOOP_LINKDEF_H
