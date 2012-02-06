/* $Id:$ */
/**
 * @file   TCatHitV1190.h
 * @date   Created : Feb 04, 2012 18:04:20 JST
 *   Last Modified : Feb 04, 2012 22:55:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATHITV1190_H
#define TCATHITV1190_H

#include <TCatObject.h>

class TCatHitV1190  : public TCatObject {

public:
   typedef enum { kInvalid = -3, kLeading, kTrailing, kValid } EStatus;

   TCatHitV1190();
   TCatHitV1190(const Int_t &cid, const Int_t &did,
                const Int_t &leading, const Int_t &trailing, 
                const EStatus &status);
   ~TCatHitV1190();


   Int_t GetLeading() { return fLeading; }
   Int_t GetTrailing() { return fTrailing; }
   Int_t GetWidth() { return fWidth; }

   EStatus GetStatus() { return fStatus; }
   
   Bool_t IsValid() { return (fStatus == kValid); }


private:
   Int_t fLeading;
   Int_t fTrailing;
   Int_t fWidth;
   EStatus fStatus;

   ClassDef(TCatHitV1190,1);
};
#endif // end of #ifdef TCATHITV1190_H
