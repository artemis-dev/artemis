/* $Id:$ */
/**
 * @file   TFileDataSource.cc
 * @date   Created : Jul 13, 2013 15:13:11 JST
 *   Last Modified : 2023-02-18 12:11:16 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TFileDataSource.h"


art::TFileDataSource::TFileDataSource(const char *filename)
   : fFile(NULL), fFilename(filename)
{
   if (stat(filename,&fFileStat) == -1) {
      fFilename = "";
      return;
   }
   fFile = fopen(fFilename,"r");

}
art::TFileDataSource::~TFileDataSource()
{
   if (fFile) fclose(fFile);
}


int art::TFileDataSource::Seek(long offset, int origin)
{
   if (!fFile) return -1;
   return fseek(fFile,offset,origin);
}

int art::TFileDataSource::Read(char *buf, const int&size)
{
   if (!fFile) return 0;
   return fread(buf,sizeof(char),size,fFile);
}


Int_t art::TFileDataSource::IsPrepared()
{
   if (!fFile || ferror(fFile)) {
      fStatus = kERROR;
      return kFALSE;
   }
   if (feof(fFile)) {
      fStatus = kEOF;
      return kFALSE;
   }
   return kTRUE;
}
