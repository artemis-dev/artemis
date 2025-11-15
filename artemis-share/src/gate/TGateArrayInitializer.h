/**
 * @file   TGateArrayInitializer.h
 * @brief
 *
 * @date   Created:       2014-01-16 12:51:42
 *         Last Modified: 2014-01-16 12:57:37
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TGATEARRAYINITIALIZER_H
#define TGATEARRAYINITIALIZER_H

#include <TProcessor.h>

namespace art {
   class TGateArrayInitializer;
   class TGateArray;
}

class art::TGateArrayInitializer : public TProcessor {
public:
   // Default constructor
   TGateArrayInitializer();
   virtual ~TGateArrayInitializer();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TGateArray *fGates;

private:
   // Copy constructor (prohibited)
   TGateArrayInitializer(const TGateArrayInitializer&);
   // Assignment operator (prohibited)
   TGateArrayInitializer& operator=(const TGateArrayInitializer&);

   ClassDef(TGateArrayInitializer,0)
};

#endif // TGATEARRAYINITIALIZER_H
