/* @file IPosition.cc
 * @brief description
 *
 * @date Create        : 2019-05-15 10:55:03 JST
 *       Last Modified : 2019-05-15 11:29:24 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "IPosition.h"

#include "TVector3.h"

ClassImp(art::IPosition)

using art::IPosition;

IPosition::IPosition()
{

}

IPosition::~IPosition()
{
}


TVector3 IPosition::Vec() const
{
   return TVector3(X(),Y(),Z());
}


void IPosition::SetXYZ(const TVector3& pos)
{
   SetX(pos.X());
   SetY(pos.Y());
   SetZ(pos.Z());
}
