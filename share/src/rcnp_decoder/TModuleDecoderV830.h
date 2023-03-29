/**
 * @file   TModuleDecoderV830.h
 * @brief  Decoder Class for V830
 *
 * @date   Created:       2013-07-24 14:31:09
 *         Last Modified: 2013-07-25 16:15:17
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERV830_H
#define TMODULEDECODERV830_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV830;
}

class art::TModuleDecoderV830 : public TModuleDecoder{
  public:
  static const int kID=8;
   // Default constructor
   TModuleDecoderV830();
   // Default destructor
   virtual ~TModuleDecoderV830();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

protected:
   TObjArray *fHitData;

private:
   // Copy constructor (prohibited)
   TModuleDecoderV830(const TModuleDecoderV830& rhs);
   // Assignment operator (prohibited)
   TModuleDecoderV830& operator=(const TModuleDecoderV830& rhs);

   ClassDef(TModuleDecoderV830,0);
};

#endif // TMODULEDECODERV830_H
