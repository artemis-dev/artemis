/* $Id:$ */
/**
 * @file   TModuleDecoderHRTDC.h
 * @date   Created : Feb 07, 2023 11:34:00 JST
 *   Last Modified : May 12, 2014 23:26:04 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2023
 */
#ifndef TMODULEDECODERHRTDC_H
#define TMODULEDECODERHRTDC_H

#include "TModuleDecoder.h"

namespace art {
   class TModuleDecoderHRTDC;
}


class art::TModuleDecoderHRTDC : public TModuleDecoder {
public:
   static const Int_t kID = 1;


   TModuleDecoderHRTDC();
   TModuleDecoderHRTDC(Int_t id);
   virtual ~TModuleDecoderHRTDC();
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);
   

   static const unsigned int kShiftHeader = 58;
   static const unsigned int kHeaderMask = 0x3f; // header mask
   static const unsigned int kHeaderTDC  = 0x0b; // tdc data
   static const unsigned int kHeaderHBD  = 0x1c; // heartbeat delimiter
   static const unsigned int kHeaderSPND = 0x18; // spill on delimiter 
   static const unsigned int kHeaderSPFD = 0x14; // spill off delimiter

   // for tdc data
   static const unsigned long long kShiftChannel = 51;
   static const unsigned long long kMaskChannel = 0x7f;
   static const unsigned long long kShiftTOT    = 43;
   static const unsigned long long kMaskTOT     = 0xff;
   static const unsigned long long kShiftTime   = 24;
   static const unsigned long long kMaskTime    = 0x7ffff;

protected:
//   TClonesArray *fHitData; //!

   ClassDef(TModuleDecoderHRTDC,1); //  decoder for high resolution TDC (HRTDC)
};

#endif // TMODULEDECODERHRTDC_H
