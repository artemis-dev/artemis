/* $Id:$ */
/**
 * @file   TCatReadoutPadArray.h
 * @date   Created : Dec 01, 2013 01:01:03 JST
 *   Last Modified : Dec 01, 2013 02:30:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATREADOUTPADARRAY_H
#define TCATREADOUTPADARRAY_H

#include <TObjArray.h>

class TH2Poly;
namespace art {
   class TCatReadoutPadArray;
   class TCatReadoutPad;
}

class art::TCatReadoutPadArray  : public TObjArray {

public:
   TCatReadoutPadArray();
   ~TCatReadoutPadArray();
   virtual void Add(TCatReadoutPad* pad) { TObjArray::Add((TObject*)pad); }

   TH2Poly *CreateHist(const char *name, const char *title,
                       Double_t xmin, Double_t xmax,
                       Double_t ymin, Double_t ymax,
                       Bool_t doFillID = kFALSE);

protected:
   virtual void Add(TObject *obj) { TObjArray::Add(obj); }

   ClassDef(TCatReadoutPadArray,1); // an array of readout pads
};
#endif // end of #ifdef TCATREADOUTPADARRAY_H
