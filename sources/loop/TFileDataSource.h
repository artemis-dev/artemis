/* $Id:$ */
/**
 * @file   TFileDataSource.h
 * @date   Created : Jul 13, 2013 15:13:46 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TFILEDATASOURCE_H
#define TFILEDATASOURCE_H

#include <TDataSource.h>
#include <sys/stat.h>

namespace art{
   class TFileDataSource;
}

class art::TFileDataSource  : public TDataSource {

public:
   TFileDataSource(const char* name);
   virtual ~TFileDataSource();

   virtual Int_t Read(Char_t *buf, const Int_t &size);
   virtual Int_t Seek(Long_t offset, Int_t origin);

protected:
   FILE *fFile;
   struct stat fFileStat;

   TString fFilename;
};
#endif // end of #ifdef TFILEDATASOURCE_H
