/* $Id:$ */
/**
 * @file   TCatStreamBuf.cc
 * @date   Created : May 02, 2012 18:02:56 JST
 *   Last Modified : May 03, 2012 11:25:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatStreamBuf.h"

#include <iostream>

TCatStreamBuf::TCatStreamBuf()
   : fMaxBufLines(kMaxBufLines)
{
}
TCatStreamBuf::~TCatStreamBuf()
{
}

void TCatStreamBuf::Buffer(const char *s, std::streamsize n)
{
   // always buffered
   for (Int_t i=0; i<n; i++) {
      fLine += s[i];
      if (s[i] == '\n') {
         if (!fMaxBufLines || fLines.size() == fMaxBufLines) {
            fLines.pop_front();
         }
         fLines.push_back(fLine);
         fLine = "";
      }
   }
}
int TCatStreamBuf::overflow(int_type ichar)
{
   char c = (char)ichar;
   Buffer(&c,1);
   return ichar;
}

std::streamsize TCatStreamBuf::xsputn(const char *s, std::streamsize n)
{
   Buffer(s,n);
   return n;
}

void TCatStreamBuf::Print()
{
   std::list<TString>::iterator it = fLines.begin();
   std::list<TString>::iterator itend = fLines.end();
   
   for (;it!=itend; it++) {
      std::cout << *it << std::flush;
   }
}

void TCatStreamBuf::SetMaxBufLines(UInt_t n)
{
   fMaxBufLines = n;
   while (fLines.size() > fMaxBufLines) {
      fLines.pop_front();
   }
}
