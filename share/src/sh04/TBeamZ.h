/**
 * @file   TBeamZ.h
 * @brief
 *
 * @date   Created:       2014-02-24 13:51:43
 *         Last Modified: 2014-02-24 13:55:45
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TBEAMZ_H
#define TBEAMZ_H

#include "TArtemisObject.h"

namespace art {
   namespace sh04 {
      class TBeamZ;
   }
}

class art::sh04::TBeamZ : public TArtemisObject {
public:
   // Default constructor
   TBeamZ();
   virtual ~TBeamZ();
   // // Copy constructor
   // TBeamZ(const TBeamZ& rhs);
   // // Assignment operator
   // TBeamZ& operator=(const TBeamZ& rhs);

   Double_t GetL() const {return fL;}
   Double_t GetR() const {return fR;}
   Double_t GetAverage() const {return fAverage;}

   void SetL(Double_t val) {fL = val;}
   void SetR(Double_t val) {fR = val;}
   void SetAverage(Double_t val) {fAverage = val;}
   
private:
   Double_t fL;
   Double_t fR;
   Double_t fAverage;

   ClassDef(TBeamZ,1) // beam z position determined from SMWDC in SHARAQ04
};

#endif // TBEAMZ_H
