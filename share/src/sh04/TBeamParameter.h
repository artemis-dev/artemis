/**
 * @file   TBeamParameter.h
 * @brief
 *
 * @date   Created:       2014-02-20 18:25:22
 *         Last Modified: 2014-06-17 14:09:43 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TBEAMPARAMETER_H
#define TBEAMPARAMETER_H


#include <TParameterObject.h>

namespace art {
   namespace sh04{
      class TBeamParameter;
   }
}

namespace YAML {
   class Node;
}

class art::sh04::TBeamParameter : public TParameterObject {
public:
   // Default constructor
   TBeamParameter();
   virtual ~TBeamParameter();
   // Copy constructor
//   TBeamParameter(const TBeamParameter& rhs);
   // Assignment operator
//   TBeamParameter& operator=(const TBeamParameter& rhs);

   Int_t GetZ() const {return fZ;}
   Int_t GetA() const {return fA;}
   Double_t GetBrho() const {return fBrho;}
   Double_t GetF6XD() const {return fF6XD;}

   void SetZ(Int_t val) { fZ = val; }
   void SetA(Int_t val) { fA = val; }
   void SetBrho(Double_t val) { fBrho = val; }
   void SetF6XD(Double_t val) { fF6XD = val; }

   void Print(Option_t*) const;

   void PrintDescriptionYAML() const;

private:
   Int_t    fZ;			// proton number of the beam
   Int_t    fA;			// mass number of the beam
   Float_t fBrho;		// brho of the beam
   Float_t fF6XD;               // (x,d) at F6PPAC

   ClassDef(TBeamParameter,2) // Beam parameter for SHARAQ04
};

#endif // TBEAMPARAMETER_H
