/* $Id:$ */
/**
 * @file   TModuleDecoderV1740.h
 * @date   Created : Feb 06, 2013 15:06:14 JST
 *   Last Modified : Feb 14, 2014 20:06:21 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODERV1740_H
#define TMODULEDECODERV1740_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV1740;
}

class art::TModuleDecoderV1740  : public TModuleDecoder {
public:
   static const int kID = 63;
protected:

public:
   TModuleDecoderV1740();
   virtual ~TModuleDecoderV1740();
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);
   typedef struct V1740EventHeader {
      V1740EventHeader(int val) { header = val; }
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
   } V1740EventHeader_t;

   ClassDef(TModuleDecoderV1740,0); // decoder for V1740
};
#endif // end of #ifdef TMODULEDECODERV1740_H
