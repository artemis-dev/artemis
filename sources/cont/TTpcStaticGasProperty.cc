/* @file TTpcStaticGasProperty.cc
 * @brief store the static gas property of tpc
 *
 * @date Create        : 2020-04-23 15:03:57 JST
 *       Last Modified : 2020-05-21 18:53:14 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TTpcStaticGasProperty.h"
#include "TArtSystemOfUnit.h"
#include "TMath.h"

ClassImp(art::TTpcStaticGasProperty)

using art::TTpcStaticGasProperty;
using TArtSystemOfUnit::cm;
using TArtSystemOfUnit::us;


TTpcStaticGasProperty::TTpcStaticGasProperty()
{

}

TTpcStaticGasProperty::~TTpcStaticGasProperty()
{
}



void TTpcStaticGasProperty::Print(Option_t *) const
{
   printf(" %s (%s)  \n",GetName(),Class_Name());
   printf("   fDriftVelocity  = %f (mm/ns)  = %f (cm/us)\n",fDriftVelocity,fDriftVelocity/(cm/us));
   printf("   fTransDiffCoeff = %f (mm^1/2) = %f (cm^1/2)\n",fTransDiffCoeff,fTransDiffCoeff/TMath::Sqrt(cm));
   printf("   fLongDiffCoeff  = %f (mm^1/2) = %f (cm^1/2)\n",fLongDiffCoeff,fLongDiffCoeff/TMath::Sqrt(cm));
   printf("   fConstTransDiff = %f (mm)\n",fConstTransDiff);
   printf("   fConstLongDiff  = %f (mm)\n",fConstLongDiff );
   printf("   fAttachCoeff    = %f (1/mm)\n",fAttachCoeff   );
   printf("   GetDriftLength(1000 ns) = %f (mm)\n",GetDriftLength(1000));
   printf("   GetTransDiff(100 mm)    = %f (mm)\n",GetTransDiff(100));
   printf("   GetLongDiff(100 mm)     = %f (mm)\n",GetLongDiff(100));
   printf("   GetAliveRatio(100 mm)   = %f \n",GetAliveRatio(100));
   
}

