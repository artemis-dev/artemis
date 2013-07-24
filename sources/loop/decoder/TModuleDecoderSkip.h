/**
 * @file   TModuleDecoderSkip.h
 * @brief
 *
 * @date   Created:       2013-07-24 17:29:34
 *         Last Modified: 2013-07-24 17:39:16
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERSKIP_H
#define TMODULEDECODERSKIP_H

#include <TModuleDecoder.h>

namespace art{
   class TModuleDecoderSkip;
}

class art::TModuleDecoderSkip : public TModuleDecoder {
  public:
   // Default constructor
   TModuleDecoderSkip(const Int_t id);
   // Default destructor
   virtual ~TModuleDecoderSkip();
   // Copy constructor
   TModuleDecoderSkip(const TModuleDecoderSkip& rhs);
   // Assignment operator
   TModuleDecoderSkip& operator=(const TModuleDecoderSkip& rhs);

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg){return 0;}

private:
   Int_t fID;

};

#endif // TMODULEDECODERSKIP_H
