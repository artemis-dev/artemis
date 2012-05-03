/* $Id:$ */
/**
 * @file   TCatOstream.cc
 * @date   Created : May 03, 2012 08:03:23 JST
 *   Last Modified : May 03, 2012 11:19:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatOstream.h"

TCatOstream::TCatOstream()
   : std::ostream(fBuffer = new TCatStreamBuf) {
}
TCatOstream::~TCatOstream()
{
   delete fBuffer;
}


void TCatOstream::SetBufType(EBufType_t type)
{
   fBufType = type;
   switch (fBufType) {
   case kBUFFERED:
      rdbuf(fBuffer);
      break;
   case kSTDOUT:
      rdbuf(std::cout.rdbuf());
      break;
   default:
      break;
   }
}


void TCatOstream::SetMaxBufLines(Int_t maxbuflines)
{
   fBuffer->SetMaxBufLines(maxbuflines);
}
