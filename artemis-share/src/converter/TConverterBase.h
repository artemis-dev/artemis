/**
 * @file   TConverterBase.h
 * @brief  Converter base class
 *
 * @date   Created:       2013-10-14 12:09:13
 *         Last Modified: 2014-03-03 10:41:27 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TCONVERTERBASE_H
#define TCONVERTERBASE_H

#include "TParameterObject.h"

namespace art{
   class TConverterBase;
}

class art::TConverterBase : public TParameterObject {
  public:
   // Default constructor
   TConverterBase();
   virtual ~TConverterBase();

   virtual Double_t Convert(Double_t val) const = 0;

   ClassDef(TConverterBase,1) // converter base class
};

#endif // TCONVERTERBASE_H
