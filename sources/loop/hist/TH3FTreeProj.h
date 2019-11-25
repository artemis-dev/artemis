/**
 * @file   TH3FTreeProj.h
 * @brief  Tree projection 3-D histogram
 *
 * @date   Created       : 2014-03-07 14:29:37 JST
 *         Last Modified : 2019-11-25 18:00:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_E200A523_D97B_4D78_8341_20315AECD917
#define INCLUDE_GUARD_UUID_E200A523_D97B_4D78_8341_20315AECD917

#include "TH3F.h"
#include "TAttTreeProj.h"

namespace art {
   class TH3FTreeProj;
}

class art::TH3FTreeProj : public TH3F, public TAttTreeProj {
public:
   TH3FTreeProj();
   TH3FTreeProj(const char *name, const char *title, const char *cut);
   virtual ~TH3FTreeProj();

   TH3FTreeProj(const TH3FTreeProj& rhs);
   TH3FTreeProj& operator=(const TH3FTreeProj& rhs);

   virtual void Copy(TObject &obj) const;
   
protected:

private:

   ClassDef(TH3FTreeProj,1) // Tree projection 3-D histogram
};

#endif // INCLUDE_GUARD_UUID_E200A523_D97B_4D78_8341_20315AECD917
