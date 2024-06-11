/* $Id:$ */
/**
 * @file   TFileDataSource.h
 * @date   Created : Jul 13, 2013 15:13:46 JST
 *   Last Modified : 2024-06-09 17:55:05 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */


#ifndef TZMQDATASOURCE_H
#define TZMQDATASOURCE_H

#include "TDataSource.h"
#include "TZmqSubscriber.h"


namespace art{
   class TZmqDataSource;
   class TZmqSubscriber;
}

class art::TZmqDataSource  : public TDataSource {
public:   

public:
   // @param uri zmq uri
   // @param reado timeout for readout in milisecond 
   TZmqDataSource();
   virtual ~TZmqDataSource();

   virtual Int_t Read(Char_t *buf, const Int_t &size) override;
   virtual Int_t Seek(Long_t offset, Int_t origin) override;

   virtual void SetURI(const std::string& uri) { fSubscriber->SetUri(uri); }
   virtual void SetReadoutTimeout(int time) { fSubscriber->SetReadoutTimeout(time); }

   virtual Int_t IsPrepared() override;
   virtual bool Prepare();


protected:
   TZmqSubscriber* fSubscriber;
   int fConnectionTimeout;
   int fMemoryPos;
};

#endif // TZMQDATASOURCE_H
