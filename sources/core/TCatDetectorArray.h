/* $Id:$ */
/**
 * @file   TCatDetectorArray.h
 * @date   Created : Feb 04, 2012 16:04:25 JST
 *   Last Modified : Feb 05, 2012 11:40:05 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATDETECTORARRAY_H
#define TCATDETECTORARRAY_H

#include <TClonesArray.h>
#include <TCatObject.h>


class TCatDetectorArray : public TCatObject {
public:
   TCatDetectorArray();

   TCatDetectorArray(const TCatDetectorArray &rhs) ;


   virtual ~TCatDetectorArray();

   virtual void Clear(const Option_t * /* opt */ = "") {
      fHits->Clear("C");
      fNumHits = 0;
   }

   virtual void Init() {;}
   virtual void Sort() { fHits->Sort(); }
   virtual Int_t GetNumHits() const { return fNumHits; }
   
protected:
   Int_t         fNumHits;
   TClonesArray *fHits;
   ClassDef(TCatDetectorArray,1);
};
#endif // end of #ifdef TCATDETECTORARRAY_H
