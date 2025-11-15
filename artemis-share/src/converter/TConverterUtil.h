/**
 * @file   TConverterUtil.h
 * @brief  Utility class for Converter Use
 *
 * @date   Created:       2013-11-08 09:46:15
 *         Last Modified: 2013-12-02 16:27:12
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TCONVERTERUTIL_H
#define TCONVERTERUTIL_H

#include <vector>
#include <TArtTypes.h>

namespace art {
   class TConverterUtil;
   class TEventCollection;
   class TConverterBase;
}

class TString;

class art::TConverterUtil {
public:
   static void SetConverterArray(std::vector<TConverterBase*> **p,
				 const TString &name, TEventCollection* const col);

   static void SetConverterArrayFromList(std::vector<TConverterBase*> **p,
					 const StringVec_t &nameList,
					 TEventCollection* const col);

private:
   TConverterUtil(); // prohibit instantiation
};

#endif // TCONVERTERUTIL_H
