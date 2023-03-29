/**
 * @file   TArtMathUtil.h
 * @brief  mathemtica utility
 *
 * @date   Created       : 2018-05-24 18:16:16 JST
 *         Last Modified : 2018-09-05 19:31:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_76DE4D52_3BF7_4955_AE61_6867FA1A61B3
#define INCLUDE_GUARD_UUID_76DE4D52_3BF7_4955_AE61_6867FA1A61B3

#include <Rtypes.h>
#include "TMath.h"

namespace art {
   namespace TArtMathUtil {
      Bool_t IsWithinRange(Double_t val, Double_t min, Double_t max);

      inline Double_t Polynomial(Double_t x, Double_t *p, Int_t nCoeff) {
         Double_t sum = 0;
         for (Int_t i = nCoeff - 1; i >= 0; --i) {
            sum += p[i] + x * sum;
         }
         return sum;
      }

      inline void GetClosestPoint2D(Double_t startX, Double_t startY, Double_t endX, Double_t endY,
                                    Double_t pointX, Double_t pointY, Double_t& closestX, Double_t& closestY);
   };
};

void art::TArtMathUtil::GetClosestPoint2D(Double_t startX, Double_t startY, Double_t endX, Double_t endY,
                                          Double_t pointX, Double_t pointY, Double_t& closestX, Double_t& closestY)

{
   const Double_t vx1 = endX - startX;
   const Double_t vy1 = endY - startY;
   const Double_t vx2 = pointX - startX;
   const Double_t vy2 = pointY - startY;
   const Double_t mag = (vx1 * vx2 + vy1 * vy2) / ( vx1 * vx1 + vy1 * vy1 );
   closestX = mag * vx1 + startX;
   closestY = mag * vy1 + startY; 
}

#endif // INCLUDE_GUARD_UUID_76DE4D52_3BF7_4955_AE61_6867FA1A61B3
