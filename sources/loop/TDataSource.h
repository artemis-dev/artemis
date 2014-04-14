/* $Id:$ */
/**
 * @file   TDataSource.h
 * @date   Created : Jul 12, 2013 17:12:16 JST
 *   Last Modified : Nov 30, 2013 00:58:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  ABC class for data source
 *    Copyright (C)2013
 */
#ifndef TDATASOURCE_H
#define TDATASOURCE_H

#include <TROOT.h>

namespace art {
   class TDataSource;
}

class art::TDataSource  {
public:
   TDataSource() {;}
   virtual ~TDataSource() {;}
   typedef enum { kERROR, kReady, kWAIT, kEOF } EStatus_t ;

   virtual Int_t IsPrepared() { return kTRUE; }
   virtual Int_t Read(Char_t *buf, const Int_t &size) = 0;
   virtual Int_t Seek(Long_t offset, Int_t origin) = 0;
   virtual EStatus_t GetStatus() { return fStatus; }
   virtual void Lock() {}
   virtual void Unlock() {}

protected:
   EStatus_t fStatus;
   
};
#endif // end of #ifdef TDATASOURCE_H
