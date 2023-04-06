/**
 * @file   TPolynomialConverterGenerator.h
 * @brief
 *
 * @date   Created:       2013-12-05 17:01:02
 *         Last Modified: 2014-01-10 14:56:53
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TPOLYNOMIALCONVERTERGENERATOR_H
#define TPOLYNOMIALCONVERTERGENERATOR_H

#include <TProcessor.h>

namespace art {
   class TPolynomialConverterGenerator;
}

class TList;

class art::TPolynomialConverterGenerator : public TProcessor {
public:
   // Default constructor
   TPolynomialConverterGenerator();
   virtual ~TPolynomialConverterGenerator();

   virtual void Init(TEventCollection *col);

private:
   TList      *fConverterList; //!
   StringVec_t fParameterList;

   // Copy constructor (prohibited)
   TPolynomialConverterGenerator(const TPolynomialConverterGenerator&);
   // Assignment operator (prohibited)
   TPolynomialConverterGenerator& operator=(const TPolynomialConverterGenerator&);

   ClassDef(TPolynomialConverterGenerator,0) // generator of polynomial converter
};

#endif // TPOLYNOMIALCONVERTERGENERATOR_H
