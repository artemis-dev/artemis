/**
 * @file   TPolynomialConverterArrayGenerator.h
 * @brief
 *
 * @date   Created:       2013-10-13 16:19:47
 *         Last Modified: 2014-01-10 14:29:21
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TPOLYNOMIALCONVERTERARRAYGENERATOR_H
#define TPOLYNOMIALCONVERTERARRAYGENERATOR_H

#include <TProcessor.h>

namespace art{
   class TPolynomialConverterArrayGenerator;
}

class TList;

class art::TPolynomialConverterArrayGenerator : public TProcessor {
public:
   // Default constructor
   TPolynomialConverterArrayGenerator();
   virtual ~TPolynomialConverterArrayGenerator();

   virtual void Init(TEventCollection *col);

private:
   TList        *fConverterArrayList; // generated converters are owned by this TList
   StringVec_t   fParameterList;
   
   // Copy constructor (prohibited)
   TPolynomialConverterArrayGenerator(const TPolynomialConverterArrayGenerator& rhs);
   // Assignment operator (prohibited)
   TPolynomialConverterArrayGenerator& operator=(const TPolynomialConverterArrayGenerator& rhs);

   ClassDef(TPolynomialConverterArrayGenerator,0) // generator of array of polynomial converter
};

#endif // TPOLYNOMIALCONVERTERARRAYGENERATOR_H
