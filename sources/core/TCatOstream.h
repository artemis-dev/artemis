/* $Id:$ */
/**
 * @file   TCatOstream.h
 * @date   Created : May 03, 2012 08:03:55 JST
 *   Last Modified : May 03, 2012 11:19:03 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATOSTREAM_H
#define TCATOSTREAM_H

#include <TCatStreamBuf.h>

class TCatOstream : public std::ostream {

public:
   typedef enum { kBUFFERED, kSTDOUT, kFILE } EBufType_t;
   TCatOstream();
   ~TCatOstream();
   void Print() {
      fBuffer->Print();
   }
   
   void SetBufType(EBufType_t type = kBUFFERED);
   void SetMaxBufLines(Int_t maxbuflines);
   
private:
   EBufType_t fBufType;
   TCatStreamBuf *fBuffer;
};
#endif // end of #ifdef TCATOSTREAM_H
