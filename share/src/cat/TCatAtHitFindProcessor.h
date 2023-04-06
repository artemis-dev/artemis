/* $Id:$ */
/**
 * @file   TCatAtHitFindProcessor.h
 * @date   Created : Feb 11, 2013 01:11:32 JST
 *   Last Modified : Feb 11, 2013 01:08:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATHITFINDPROCESSOR_H
#define TCATATHITFINDPROCESSOR_H

class TCatAtHitFindProcessor  : public TProcessor {

public:
   TCatAtHitFindProcessor();
   ~TCatAtHitFindProcessor();


   void Init(TCatEventCollection *);
   void Process();
   
   StringVec_t fInputColName;
   TString     fOutputName;
   
   TClonesArray *fIn;
   TClonesArray *fOut;
};
#endif // end of #ifdef TCATATHITFINDPROCESSOR_H
