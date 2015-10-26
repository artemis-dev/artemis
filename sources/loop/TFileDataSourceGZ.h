/**
 * @file   TFileDataSourceGZ.h
 * @brief  file data source with gzip compression
 *
 * @date   Created       : 2015-04-29 18:32:57 JST
 *         Last Modified : Oct 26, 2015 06:37:56 EDT
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_CB07EBDF_7787_4F57_9726_3AB250737A0C
#define INCLUDE_GUARD_UUID_CB07EBDF_7787_4F57_9726_3AB250737A0C

#include "TDataSource.h"
#include <zlib.h>
#include <sys/stat.h>

namespace art {
   class TFileDataSourceGZ;
}

class art::TFileDataSourceGZ : public TDataSource {
public:
   TFileDataSourceGZ(const char* name = NULL);
   virtual ~TFileDataSourceGZ();

   TFileDataSourceGZ(const TFileDataSourceGZ& rhs);
   TFileDataSourceGZ& operator=(const TFileDataSourceGZ& rhs);

   virtual Int_t Read(Char_t *buf, const Int_t &size);
   virtual Int_t Seek(Long_t offset, Int_t origin);
   

protected:
   gzFile fFile; // pointer to gzip file
   struct stat fFileStat; // file status

   TString fFilename; // filename
private:

//   ClassDef(TFileDataSourceGZ,0) // file data source with gzip compression
};

#endif // INCLUDE_GUARD_UUID_CB07EBDF_7787_4F57_9726_3AB250737A0C
