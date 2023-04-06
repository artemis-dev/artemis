/* $Id:$ */
/**
 * @file   TCatV1740AdcCorrectionProcessor.cc
 * @date   Created : Mar 10, 2013 17:10:15 JST
 *   Last Modified : Mar 18, 2013 14:32:46 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatV1740AdcCorrectionProcessor.h"

#include <TCatFadcRawData.h>

TCatV1740AdcCorrectionProcessor::TCatV1740AdcCorrectionProcessor()
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","fadc rawdata (TCatFadcRawData)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatRadcRawData)",
                            fOutputColName,TString("atrawcor"));
   RegisterProcessorParameter("Rejection","Reject Invalid (0xfxx) like signal (1) or not (0)",
                            fDoRejectInvalid,0);
   fCatHitOut = new TClonesArray("TCatFadcRawData");
}
TCatV1740AdcCorrectionProcessor::~TCatV1740AdcCorrectionProcessor()
{
   delete fCatHitOut;
}
void TCatV1740AdcCorrectionProcessor::Init(TCatEventCollection *col)
{
   fCatHit = dynamic_cast<TClonesArray*>(col->Get(fInputColName.front()));
   fCatHitOut->Clear("C");
   fCatHitOut->SetName(fOutputColName);
   col->Add(fCatHitOut,fOutputIsTransparent);
}

void TCatV1740AdcCorrectionProcessor::Process()
{
   fCatHitOut->Clear("C");
   TIter next(fCatHit);
   TObject *obj = NULL;
   while ((obj = next())) {
      TCatFadcRawData *hit = (TCatFadcRawData*)obj;
      TCatFadcRawData *dest = (TCatFadcRawData*) fCatHitOut->ConstructedAt(fCatHitOut->GetEntriesFast());
      dest->SetID(hit->GetID());
      dest->SetTimestamp(hit->GetTimestamp());
      dest->SetGeo(hit->GetGeo());
      dest->SetCh(hit->GetCh());
      dest->SetOffset(hit->GetOffset());
      dest->SetPattern(hit->GetPattern());
      Int_t nSample = hit->GetNumSample();
      for (Int_t i=0; i!=nSample; i++) {
         UShort_t val = (*hit)[i];
         UShort_t val2,val3;
         if ((0xf00 & val)==0xf00) {
            if (fDoRejectInvalid) continue;
               
            if (i == 0) {
               val2 = (*hit)[i+1];
               val3 = (*hit)[i+2];
            } else if (i==nSample-1) {
               val2 = (*hit)[i-1];
               val3 = (*hit)[i-2];
            } else {
               val2 = (*hit)[i-1];
               val3 = (*hit)[i+1];
            }
            if (0 && abs(val2-val3) < 2) {
               if ((val&val2)==val2) {
                  val = val2;
               } else if ((val&val3)==val3) {
                  val = val3;
               } 
            } else {
//            val -= (0xff0&(val&(~(val2))));
//               Int_t iShift = 0;
//               Int_t vm = ((val2+val3)>>1) - 0x800;
//               for (iShift = 0; iShift < 12; iShift++) {
//                  if (((vm << iShift)&0x800) == 0x800) break;
//               }
//               iShift = 12-iShift;
//               Int_t mask = 0x800;
//               for (Int_t ii=0; ii<iShift; ii++) {
//                  mask += (1<<ii);
//               }
//               val -= ((val - (val2+val3)/2)&(0xfff&~abs(val2-val3)));
//               val = (val & mask);
//               val -= ((0xfff&~(abs(val2-val3)>>1))&(val&(~((val2+val3)>>1))));
//               if (val == 0xfff) {
//                  val = (val2 + val3)>>1;
//               } else
               if ((val&0xff0) == 0xff0) {
                  UShort_t v = (val&0xff0);
                  UShort_t v2 = (val2);
                  UShort_t v3 = (val3);
//                  v -= (((0xff0&(~abs(v2-v3)>>1))&(v&~((v2+v3)>>1))));
                  v = (v & ((v2+v3)>>1));
                  val = (val&0xf) + v;
               } else {
//                  val -= ((0xfff&~(abs(val2-val3)>>1))&(val&(~((val2+val3)>>1))));
//                  val-=(((0xfff&(~abs(val2-val3)>>1))&(val&~((val2+val3)>>1))));
                  UShort_t v = (val&0xf00);
                  UShort_t v2 = (val2);
                  UShort_t v3 = (val3);
//                  v -= (((0xf00&(~abs(v2-v3)>>1))&(v&~((v2+v3)>>1))));
                  v = (v & ((v2+v3)>>1));
                  val = (val&0xff) + v;
//                  UShort_t mean = ((val2+val3)>>1);
//                  Int_t ii;
//                  UShort_t mask = 0;
//                  for (ii=0; ii<11; ii++) {
//                     if ((mean) & (1<<(11-ii))) break;
//                  }
//                  for (Int_t iii = 0; iii<ii;iii++) {
//                     
//                  }
//                  val = (val & (((val2+val3)>>1)|0xf));
               }
            }
         }
         dest->AddSample(val,i);
      }
   }
}



