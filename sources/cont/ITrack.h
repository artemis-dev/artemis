/* @file ITrack.h
 * @brief interface for track
 *
 * @date Create        : 2019-06-08 23:09:26 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_341125DE_BCC7_4B7A_8AA5_9A2B9BECB2E4
#define INCLUDE_GUARD_UUID_341125DE_BCC7_4B7A_8AA5_9A2B9BECB2E4

#include "Rtypes.h"

namespace art {
   class ITrack;
}

class TVector3;

class art::ITrack {
public:
   virtual ~ITrack();

   virtual const TVector3& GetStartPoint() const = 0;
   virtual const TVector3& GetEndPoint() const = 0;
   virtual const TVector3& GetDirection() const = 0;
   virtual void GetStartPoint(double& x, double& y, double& z) const = 0;
   virtual void GetEndPoint(double& x, double& y, double& z) const = 0;
   virtual void GetDirection(double& x, double& y, double& z) const = 0;
   
   virtual void SetTrack(double x1, double y1, double z1, double x2, double y2, double z2) = 0;
   virtual void SetTrack(const TVector3& start, const TVector3& end) = 0;
   
   ClassDef(ITrack,1)
}; // end of ITrack

#endif // #ifndef INCLUDE_GUARD_UUID_341125DE_BCC7_4B7A_8AA5_9A2B9BECB2E4}
