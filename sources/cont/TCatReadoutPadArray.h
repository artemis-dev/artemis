/* $Id:$ */
/**
 * @file   TCatReadoutPadArray.h
 * @date   Created : Dec 01, 2013 01:01:03 JST
 *   Last Modified : 2016-11-11 18:02:50 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATREADOUTPADARRAY_H
#define TCATREADOUTPADARRAY_H

#include <TObjArray.h>
#include <vector>

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

   virtual void SetBads(const std::vector<Int_t> &bads);
   virtual void SetLowGains(const std::vector<Int_t> &lowGains);

   virtual const std::vector<Int_t>& Bads() { return fBads; }
   virtual const std::vector<Int_t>& LowGains() { return fLowGains; }

protected:
   virtual void Add(TObject *obj) { TObjArray::Add(obj); }

   std::vector<Int_t> fBads; //
   std::vector<Int_t> fLowGains; //

   ClassDef(TCatReadoutPadArray,1); // an array of readout pads
};
#endif // end of #ifdef TCATREADOUTPADARRAY_H
