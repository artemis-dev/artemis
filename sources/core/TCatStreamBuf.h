/* $Id:$ */
/**
 * @file   TCatStreamBuf.h
 * @date   Created : May 02, 2012 18:02:41 JST
 *   Last Modified : May 03, 2012 11:14:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSTREAMBUF_H
#define TCATSTREAMBUF_H

#include <list>
#include <streambuf>
#include <iostream>

#include <TString.h>

class TCatStreamBuf : public std::streambuf {
public:

   static const Int_t kMaxBufLines = 1000;
   TCatStreamBuf();
   virtual ~TCatStreamBuf();
   // override 
   virtual std::streamsize xsputn(const char* s, std::streamsize n);
   virtual int_type overflow(int_type /* ichar */ = EOF);
   // 
   virtual void Buffer(const char* s, std::streamsize n);
   virtual void Print();
   
   virtual void SetMaxBufLines(UInt_t n = kMaxBufLines);

private:
   TString fLine;
   std::list<TString> fLines;
   
   UInt_t      fMaxBufLines;
};

#endif // end of #ifdef TCATSTREAMBUF_H
