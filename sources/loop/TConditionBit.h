/* $Id:$ */
/**
 * @file   TConditionBit.h
 * @date   Created : Jul 10, 2013 13:10:52 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TCONDITIONBIT_H
#define ART_TCONDITIONBIT_H

#include <TObject.h>

namespace art {
   class TConditionBit;
}

class art::TConditionBit  : public TObject {

public:
   TConditionBit();
   ~TConditionBit();

   void Set(const UInt_t& bit)   { fValue |= (1<<bit); }
   void Unset(const UInt_t& bit) { fValue &= ((0xffffffff)^(1<<bit)); }

   Bool_t IsSet(const UInt_t& bit) { return (fValue & (1<<bit)); }
   Bool_t IsSetMask(const UInt_t& mask) { return (fValue & mask); }

   static UInt_t Bit(const UInt_t& bit) { return (1<<bit); }

private:
   UInt_t fValue;

   ClassDef(TConditionBit,1);
};
#endif // end of #ifdef ART_TCONDITIONBIT_H
