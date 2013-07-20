/* $Id:$ */
/**
 * @file   TRawDataV1190.h
 * @date   Created : Jul 20, 2013 14:20:52 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRAWDATAV1190_H
#define TRAWDATAV1190_H

#include <TRawDataObject.h>

namespace art {
   class TRawDataV1190;
}

class art::TRawDataV1190  : public TRawDataObject {

public:
   TRawDataV1190();
   ~TRawDataV1190();

   void SetLeading(const Int_t &val) { fLeading = val; }
   void SetTrailing(const Int_t &val) { fTrailing = val; }

   Int_t GetLeading() { return fLeading; }
   Int_t GetTrailing() { return fTrailing; }
private:
   Int_t fLeading; // leading edge timing
   Int_t fTrailing; //  trailing or width

   ClassDef(TRawDataV1190,1);
};
#endif // end of #ifdef TRAWDATAV1190_H
