/* $Id:$ */
/**
 * @file   artstreaming_linkdef.h
 * @date   Created : Feb 07, 2023 22:53:00 JST
 *   Last Modified : 2023-02-17 18:21:46 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ARTSTREAMING_LINKDEF_H
#define ARTSTREAMING_LINKDEF_H

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


//#pragma link C++ class art::TModuleDecoderHRTDC;
#pragma link C++ class art::TStreamingHeartBeatDelimiter;
#pragma link C++ class art::TStreamingSpillDelimiter;
#pragma link C++ class art::TStreamingEventStore;
#pragma link C++ class art::TStreamingHeaderSTF;
#pragma link C++ class art::TStreamingHeaderFS;
#pragma link C++ class art::TStreamingHeaderTF;
#pragma link C++ class art::TStreamingHeaderFLTCOIN;


// #pragma link C++ class art::TSTREAMINGEventStore::STREAMINGCommentRunInfo;



#endif /* __CINT__ */

#endif // end of #ifdef ARTRIDF_LINKDEF_H
