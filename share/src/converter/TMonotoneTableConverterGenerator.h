/**
 * @file   TMonotoneTableConverterGenerator.h
 * @brief
 *
 * @date   Created:       2013-10-18 10:27:18
 *         Last Modified: 2014-01-09 18:39:42
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMONOTONETABLECONVERTERGENERATOR_H
#define TMONOTONETABLECONVERTERGENERATOR_H

#include <TProcessor.h>

namespace art {
   class TMonotoneTableConverterGenerator;
}

class TList;

class art::TMonotoneTableConverterGenerator : public TProcessor {
public:
   // Default constructor
   TMonotoneTableConverterGenerator();
   virtual ~TMonotoneTableConverterGenerator();

   virtual void Init(TEventCollection *col);

private:
   TList      *fConverterList; // generated converters are owned by this list
   StringVec_t fParameterList;

   // Copy constructor (prohibited)
   TMonotoneTableConverterGenerator(const TMonotoneTableConverterGenerator& rhs);
   // Assignment operator (prohibited)
   TMonotoneTableConverterGenerator& operator=(const TMonotoneTableConverterGenerator& rhs);

   ClassDef(TMonotoneTableConverterGenerator,0) // generator of monotone table converter
};

#endif // TMONOTONETABLECONVERTERGENERATOR_H
