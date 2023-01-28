/* @file TTpcStaticGasProperty.h
 * @brief Container of tpc electrostatic property
 *
 * @date Create        : 2020-04-23 14:34:27 JST
 *       Last Modified : 2020-05-21 18:53:20 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_EEAD83DA_BF63_4A29_AE06_8227464CACEB
#define INCLUDE_GUARD_UUID_EEAD83DA_BF63_4A29_AE06_8227464CACEB

#include "TParameterObject.h"
#include "TMath.h"

namespace art {
   class TTpcStaticGasProperty;
}


class art::TTpcStaticGasProperty : public TNamed {
public:
   TTpcStaticGasProperty();
   virtual ~TTpcStaticGasProperty();

   inline double GetDriftLength(double time) const;
   inline double GetTransDiff(double length) const;
   inline double GetLongDiff(double length) const;
   inline double GetAliveRatio(double length) const;
   inline double GetNumElectrion(double energy /* MeV */) const;
   inline double GetEnergy(double numElectrons) const;

   double GetDriftVelocity()  const { return fDriftVelocity  ; } // get drift velocity (mm/ns)
   double GetTransDiffCoeff() const { return fTransDiffCoeff ; } // get Transverse diffusion coefficient (sqrt(mm))
   double GetLongDiffCoeff()  const { return fLongDiffCoeff  ; } // get longitudinal diffusion coefficient (sqrt(mm))
   double GetConstTransDiff() const { return fConstTransDiff ; } // get constant transverse diffusion (mm)
   double GetConstLongDiff()  const { return fConstLongDiff  ; } // get constant transverse diffusion (mm)
   double GetAttachCoeff()    const { return fAttachCoeff    ; } // get attachment coefficient  (1/mm)
   double GetWorkFunction()   const { return fWorkFunction   ; } // get work function

   void SetDriftVelocity  (double val) { fDriftVelocity   = val; } // set drift velocity (mm/ns)
   void SetTransDiffCoeff (double val) { fTransDiffCoeff  = val; } // set Transverse diffusion coefficient (sqrt(mm))
   void SetLongDiffCoeff  (double val) { fLongDiffCoeff   = val; } // set longitudinal diffusion coefficient (sqrt(mm))
   void SetConstTransDiff (double val) { fConstTransDiff  = val; } // set constant transverse diffusion (mm)
   void SetConstLongDiff  (double val) { fConstLongDiff   = val; } // set constant transverse diffusion (mm)
   void SetAttachCoeff    (double val) { fAttachCoeff     = val; } // set attachment coefficient (1/mm)
   void SetWorkFunction   (double val) { fWorkFunction    = val; }

      

   virtual void Print(Option_t * = "") const;
   


protected:
   double fDriftVelocity  ; // drift velocity (mm/ns)
   double fTransDiffCoeff ; // Transverse diffusion coefficient (sqrt(mm))
   double fLongDiffCoeff  ; // longitudinal diffusion coefficient (sqrt(mm))
   double fConstTransDiff ; // constant transverse diffusion (mm)
   double fConstLongDiff  ; // constant transverse diffusion (mm)
   double fAttachCoeff    ; // attachment coefficient (1/mm)
   double fWorkFunction   ; // work function (MeV)

   ClassDef(TTpcStaticGasProperty,1)
}; // end of TTpcStaticGasProperty

double art::TTpcStaticGasProperty::GetDriftLength(double time) const
{
   return fDriftVelocity * time;
}

double art::TTpcStaticGasProperty::GetTransDiff(double length) const
{
   const double diff = fTransDiffCoeff*TMath::Sqrt(length);
   return TMath::Sqrt(diff * diff + fConstTransDiff * fConstTransDiff);
}
double art::TTpcStaticGasProperty::GetLongDiff(double length) const
{
   const double diff = fLongDiffCoeff*TMath::Sqrt(length);
   return TMath::Sqrt(diff * diff + fConstLongDiff * fConstLongDiff);
}

double art::TTpcStaticGasProperty::GetAliveRatio(double length) const
{
   return TMath::Exp(- fAttachCoeff * length);
}

double art::TTpcStaticGasProperty::GetNumElectrion(double energy) const
{
   return energy / fWorkFunction;
}

double art::TTpcStaticGasProperty::GetEnergy(double numElectrons) const
{
   return numElectrons * fWorkFunction;
}

#endif // #ifndef INCLUDE_GUARD_UUID_EEAD83DA_BF63_4A29_AE06_8227464CACEB}
