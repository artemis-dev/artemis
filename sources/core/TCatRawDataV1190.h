/* $Id:$ */
/**
 * @file   TCatRawDataV1190.h
 * @date   Created : Jun 22, 2012 19:22:59 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATRAWDATAV1190_H
#define TCATRAWDATAV1190_H

#include <TCatObject.h>

class TCatRawDataV1190  : public TCatObject {

public:
   TCatRawDataV1190();
   TCatRawDataV1190(Int_t igeo, Int_t ich);
   ~TCatRawDataV1190();
   Int_t RawLeading() { return fLeading; }
   Int_t RawTrailing() { return fTrailing; }
   Int_t Leading() { return fLeading - fReference; }
   Int_t Trailing() { return fTrailing - fReference; }
   Int_t Width() { return fTrailing - fLeading; }
   void SetLeading(Int_t raw) { fLeading = raw; }
   void SetTrailing(Int_t raw) { fTrailing = raw; }
   void SetReference(Int_t raw) { fReference = raw; }
   Bool_t IsValid() {
      return ((fLeading > 0) && (fTrailing >0));
   }
private:
   Int_t fReference;
   Int_t fLeading;
   Int_t fTrailing;  // trailing or width
   Int_t fWidth;
   ClassDef(TCatRawDataV1190,0);
};
#endif // end of #ifdef TCATRAWDATAV1190_H
