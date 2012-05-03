/* $Id:$ */
/**
 * @file   catloop_linkdef.h
 * @date   Created : Feb 19, 2012 11:19:06 JST
 *   Last Modified : May 03, 2012 09:52:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef CATLOOP_LINKDEF_H
#define CATLOOP_LINKDEF_H

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// #pragma link C++ class TCatEventCollectionManager-!;
#pragma link C++ class TCatProcessor;
#pragma link C++ class TCatSleepProcessor;
#pragma link C++ class TCatLoop;
#pragma link C++ class TCatLoopManager;
#pragma link C++ class TThreadPoolTaskImp<TCatLoop, EProc>;

#endif /* __CINT__ */

#endif // end of #ifdef CATLOOP_LINKDEF_H
