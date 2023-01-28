/**
 * @file   TAttTreeProj.h
 * @brief  Attribute to fill the tree projection histogram
 *
 * @date   Created       : 2014-03-03 23:29:51 JST
 *         Last Modified : 2023-01-28 20:28:39 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_58AA6F1B_85E5_4E3C_BB33_9DDE0446117F
#define INCLUDE_GUARD_UUID_58AA6F1B_85E5_4E3C_BB33_9DDE0446117F

#include "TAttCut.h"
#include <TH1.h>

namespace art {
   class TAttTreeProj;
   class TAxisTreeProj;
}

class TH2;
class TH3;
class TTreeFormulaManager;


class art::TAttTreeProj : public TAttCut {
public:
   TAttTreeProj();
   TAttTreeProj(TH1 *hist, const char* cut = "");
   virtual ~TAttTreeProj();

   // copy this object
   virtual void Copy(TAttCut &newatt) const;
   
   // fill each histogram
   inline virtual void FillProjection();
   
   // get axis definition
   virtual TAxisTreeProj* GetAxisDef(int i) { return fAxes[i]; }
      
   // get flag for async
   virtual Bool_t GetAxisAsync() { return fAxisAsync; }

   virtual void SetAxisAsync(Bool_t async) { fAxisAsync = async; }

   // set axis definition 
   virtual void SetAxisDef(UInt_t i, TAxisTreeProj* axis);

   // set title of projection
   virtual void SetProjTitle(const char *title) { fH->SetTitle(title); }

   // set name of projection
   virtual void SetProjName(const char *name) { fH->SetName(name); }

   // prepare managers and synchronize variables, this should be called before fill
   virtual void Sync();


protected:

   virtual void FillTo(TH1* hist);
   virtual void FillTo(TH2* hist);
   virtual void FillTo(TH3* hist);

   TAttTreeProj(const TAttTreeProj& rhs);
   virtual TAttTreeProj& operator=(const TAttTreeProj& rhs);
   
private:
   TH1*   fH; //! histgram owned by the outer object
   Bool_t               fAxisAsync;
   Bool_t fNeedSync; //
   std::vector<TTreeFormulaManager*> fManagers; //!
   std::vector<TAxisTreeProj*> fAxes;

   ClassDef(TAttTreeProj,1) // Attribute to fill the tree projection histogram
};

void art::TAttTreeProj::FillProjection()
{
   switch(fH->GetDimension()) {
   case 1:
      FillTo(fH);
      break;
   case 2:
      FillTo((TH2*) fH);
      break;
   case 3:
      FillTo((TH3*) fH);
      break;
   }
}

#endif // INCLUDE_GUARD_UUID_58AA6F1B_85E5_4E3C_BB33_9DDE0446117F
