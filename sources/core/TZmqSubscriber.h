/* $Id:$ */
/**
 * @file   TZmqSubscriber.h
 * @date   Created : Nov 29, 2013 21:29:50 JST
 *   Last Modified : 2024-06-09 14:27:10 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TZMQSUBSCRIBER_H
#define TZMQSUBSCRIBER_H

#include <TObject.h>

namespace art {
   class TZmqSubscriber;
}

class art::TZmqSubscriber  : public TObject {

public:
   static constexpr uint64_t kMaxBufSize {100 * 1024 * 1024};
   TZmqSubscriber();
   ~TZmqSubscriber();

   int GetSize() { return fSize; }
   int Read(char *buf, int size, int offset = 0);
   bool IsValid() { return fIsValid; }

   int Recv();
   bool Connect(const std::string& uri) {
      SetUri(uri);
      return Connect();
   }

   bool Connect();
   void Terminate();

   void SetReadoutTimeout(int time) { fReadoutTimeout = time; }
   void SetUri(const std::string& uri) { fUri = uri; }

   int GetReadoutTimeout() const { return fReadoutTimeout; }
   std::string GetUri() const { return fUri; }

protected:
   
   std::string fUri; // uri of mq
   void*   fContext; //! context
   void*   fSocket; //! socket
   char*   fBuffer; //! fBuffer
   int     fReadoutTimeout; // timeout for readout
   bool    fIsValid; // validation
   uint64_t fSize; // size of data
   
   ClassDef(TZmqSubscriber,1); // shared memory wrapper class
};
#endif // end of #ifdef TSHAREDMEMORY_H
