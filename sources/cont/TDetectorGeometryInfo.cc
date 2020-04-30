/* @file TDetectorGeometryInfo.cc
 * @brief detector geometry holder
 *
 * @date Create        : 2019-05-16 22:32:36 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TDetectorGeometryInfo.h"

ClassImp(art::TDetectorGeometryInfo)

using art::TDetectorGeometryInfo;

TDetectorGeometryInfo::TDetectorGeometryInfo()
{

}

TDetectorGeometryInfo::TDetectorGeometryInfo(int id, double x, double y, double z)
   : fID(id)
{
   fPos[0] = x;
   fPos[1] = y;
   fPos[2] = z;
}

TDetectorGeometryInfo::~TDetectorGeometryInfo()
{
}

