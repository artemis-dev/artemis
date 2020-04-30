/* $Id:$ */
/**
 * @file   TModuleDecoderSIS3820.h
 * @date   Created : Oct 24, 2013 22:39:28 JST
 *   Last Modified : 2019-11-22 17:51:59 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODERSIS3820_H
#define TMODULEDECODERSIS3820_H

#include <TModuleDecoder.h>

namespace art {
  class TModuleDecoderSIS3820;
}

class art::TModuleDecoderSIS3820  : public art::TModuleDecoder {
public:
  static const Int_t kID = 36;
  static const UInt_t fMaskMeasure = 0xffffffff;


public:
  TModuleDecoderSIS3820(Int_t id = kID);
  virtual ~TModuleDecoderSIS3820();
  virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);
  TModuleDecoderSIS3820(const TModuleDecoderSIS3820& /*rhs */) 
    : TModuleDecoder(kID) {;}

    ClassDef(TModuleDecoderSIS3820,1);
};
#endif // end of #ifdef TMODULEDECODERSIS3820_H
