/**
 * @file   TFileDataSourceGZ.cc
 * @brief  file data source with gzip compression
 *
 * @date   Created       : 2015-04-29 18:37:56 JST
 *         Last Modified : Apr 29, 2015 18:45:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TFileDataSourceGZ.h"

using art::TFileDataSourceGZ;

ClassImp(TFileDataSourceGZ)

TFileDataSourceGZ::TFileDataSourceGZ(const char *filename)
: fFile(NULL), fFilename(filename)
{
   if (stat(filename,&fFileStat) == -1) {
      fFilename = "";
      return;
   }
   fFile = gzopen(fFilename,"r");
}

TFileDataSourceGZ::~TFileDataSourceGZ()
{
   if (fFile) gzclose(fFile);
}

TFileDataSourceGZ::TFileDataSourceGZ(const TFileDataSourceGZ& rhs)
{
}

TFileDataSourceGZ& TFileDataSourceGZ::operator=(const TFileDataSourceGZ& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

int TFileDataSourceGZ::Seek(long offset, int origin)
{
   if (!fFile) return -1;
   return gzseek(fFile,offset,origin);
}

int TFileDataSourceGZ::Read(char *buf, const int&size)
{
   if (!fFile) return 0;
   return gzread(fFile,buf,sizeof(char)*size);
}
