/* $Id:$ */
/**
 * @file   TCatSortable.h
 * @date   Created : Feb 04, 2012 21:04:51 JST
 *   Last Modified : Feb 04, 2012 23:14:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSORTABLE_H
#define TCATSORTABLE_H

#include <TArtRawDataObject.h>

class TCatSortable : public TObject {

public:
   TCatSortable();
   TCatSortable(TArtRawDataObject *obj, const Double_t &val);
   ~TCatSortable();

   virtual Int_t Compare(const TObject *obj) const;
   virtual TArtRawDataObject* GetObject() { return fObj; }

   virtual Bool_t IsSortable() const { return kTRUE;}

private:
   TArtRawDataObject *fObj;
   Double_t fVal;

   ClassDef(TCatSortable,1);
};
#endif // end of #ifdef TCATSORTABLE_H
