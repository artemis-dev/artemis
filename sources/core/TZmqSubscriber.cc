/* $Id:$ */
/**
 * @file   TZmqSubscriber.cc
 * @date   Created : Nov 29, 2013 21:29:50 JST
 *   Last Modified : 2025-02-08 12:47:47 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TZmqSubscriber.h"


#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

using art::TZmqSubscriber;

TZmqSubscriber::TZmqSubscriber()
   : fUri(""), fContext(nullptr), fSocket(nullptr), fBuffer(nullptr),fIsValid(false),
     fReadoutTimeout(0)
{
   fBuffer = new char[kMaxBufSize];
}

TZmqSubscriber::~TZmqSubscriber()
{
   Terminate();
   delete [] fBuffer;
}


int TZmqSubscriber::Read(char *buf, int size, int offset)
{
   if (fSize < offset+size) {
      Error("Read","bound check required offset+size = %d for fSize(%d)",offset+size,fSize);
      return -1;
   }
   memcpy(buf, fBuffer+offset, size);
   return size;
}

bool TZmqSubscriber::Connect() {
   if (fUri.length() == 0) {
      fIsValid = false;
      return false;
   }
   fContext = zmq_ctx_new();
   fSocket  = zmq_socket(fContext,ZMQ_SUB);
   int rc = zmq_connect(fSocket,fUri.c_str());
   if (rc != 0) {
      Terminate();
      fIsValid = false;
      return false;
   }
   // set timeout to read out
   int hwm = 10;
   rc = zmq_setsockopt(fSocket, ZMQ_RCVTIMEO, &fReadoutTimeout, sizeof(fReadoutTimeout));
   rc = zmq_setsockopt(fSocket, ZMQ_RCVHWM, &hwm, sizeof(hwm));
   // subscribe all
   rc = zmq_setsockopt(fSocket, ZMQ_SUBSCRIBE, "",0);
   fIsValid = true;
   return true;
}

int TZmqSubscriber::Recv() {
   if ((fSize = zmq_recv(fSocket,fBuffer,kMaxBufSize,0)) < 0) {
      fIsValid = false;
      return fSize;
   }
   fIsValid = true;
   return fSize;
}

void TZmqSubscriber::Terminate() {
//   printf("invalidate %d\n",fIsValid);
   fIsValid = false;
//   printf("invalidate %d\n",IsValid());
   if (fSocket) {
      zmq_close(fSocket);
      fSocket = nullptr;
   }
   if (fContext) {
      zmq_ctx_destroy(fContext);
      fContext = nullptr;
   }
}
   
      
