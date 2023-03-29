/* $Id:$ */
/**
 * @file   TModuleDecoderV1740_mod.h
 * @date   Created : Feb 06, 2013 15:06:14 JST
 *   Last Modified : 2015-04-16 17:14:58 JST by tokieda
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODERV1740_MOD_H
#define TMODULEDECODERV1740_MOD_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV1740_mod;
}

class art::TModuleDecoderV1740_mod  : public TModuleDecoder {
public:
   static const int kID = 63;
protected:
   Int_t fVerboseLevel; // verbose level

public:
   TModuleDecoderV1740_mod();
   virtual ~TModuleDecoderV1740_mod();
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);
   typedef struct V1740_EventHeader {
      V1740_EventHeader(int val) { header = val; }
      Int_t header;
      void Print() {
         if ((header>>28) == 3) { 
            printf("Slot  = %d\n",(header&(0xff<16))>16);
            printf("Hit   = %d\n",(header&(0xff<8))>8);
            printf("Ch    = %d\n",(header&0xff));
         } else {
            printf("Invalid 0x%08x \n",header);
         }
      }
   } V1740_EventHeader_t;

   ClassDef(TModuleDecoderV1740_mod,0); // decoder for V1740_mod
};
#endif // end of #ifdef TMODULEDECODERV1740_MOD_H
