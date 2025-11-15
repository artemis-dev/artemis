/**
 * @file   TH2FTreeProj.h
 * @brief  Tree projection 2-D historgram
 *
 * @date   Created       : 2014-03-07 08:35:46 JST
 *         Last Modified : 2023-01-28 23:41:47 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_70D0F723_6660_46C0_B2C7_083D14D39096
#define INCLUDE_GUARD_UUID_70D0F723_6660_46C0_B2C7_083D14D39096

#include "TH2F.h"
#include "TAttTreeProj.h"

namespace art {
   class TH2FTreeProj;
}

class art::TH2FTreeProj : public TH2F, public TAttTreeProj {
public:
   TH2FTreeProj();
   TH2FTreeProj(const char *name, const char *title, const char *cut);
   virtual ~TH2FTreeProj();

   TH2FTreeProj(const TH2FTreeProj& rhs);
   TH2FTreeProj& operator=(const TH2FTreeProj& rhs);

   virtual void Copy(TObject& obj) const;
   

protected:

private:

   
   ClassDef(TH2FTreeProj,1) // Tree projection 2-D historgram
};

#endif // INCLUDE_GUARD_UUID_70D0F723_6660_46C0_B2C7_083D14D39096
