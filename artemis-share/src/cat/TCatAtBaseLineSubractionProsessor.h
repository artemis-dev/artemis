/* $Id:$ */
/**
 * @file   TCatAtBaseLineSubractionProsessor.h
 * @date   Created : Feb 11, 2013 00:11:30 JST
 *   Last Modified : Feb 11, 2013 01:04:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATBASELINESUBRACTIONPROSESSOR_H
#define TCATATBASELINESUBRACTIONPROSESSOR_H
#include <TProcessor.h>
class TCatAtBaseLineSubractionProsessor  : public TProcessor {

public:
   TCatAtBaseLineSubractionProsessor();
   ~TCatAtBaseLineSubractionProsessor();

   void Init(TCatEventCollection* col);
   void Process();

   StringVec_t fInputColName;
   TClonesArray f

};
#endif // end of #ifdef TCATATBASELINESUBRACTIONPROSESSOR_H
