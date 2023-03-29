/* @file TUnsharpMask.h
 * @brief unsharp mask
 *
 * @date Create        : 2020-07-29 14:19:52 JST
 *       Last Modified : 2020-07-29 16:07:58 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_29955026_7E9C_46BE_A7F6_D6F848296E03
#define INCLUDE_GUARD_UUID_29955026_7E9C_46BE_A7F6_D6F848296E03

#include <vector>

namespace art {
   namespace util {
      class TUnsharpMask;
   }
}


class art::util::TUnsharpMask {
public:
   TUnsharpMask(double sigma = 3, int halfNum = 10, double amount = 2., double threshold = 0.);
   virtual ~TUnsharpMask();

   virtual void operator() (const std::vector<double>& in, std::vector<double>& out);
   
protected:
   std::vector<double> fMask1;
   std::vector<double> fMask2;
   double fSigma;
   int fHalfNumPixel;
   double fAmount;
   double fThreshold;
   
}; // end of TUnsharpMask

#endif // #ifndef INCLUDE_GUARD_UUID_29955026_7E9C_46BE_A7F6_D6F848296E03}
