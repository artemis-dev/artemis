/**
 * @file   TH1FTreeProj.h
 * @brief  Tree projection histogram
 *
 * @date   Created       : 2014-03-04 13:11:54 JST
 *         Last Modified : Mar 07, 2014 09:03:07 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_9041C331_C91A_46F6_94BC_7A22034D2591
#define INCLUDE_GUARD_UUID_9041C331_C91A_46F6_94BC_7A22034D2591

#include "TH1F.h"
#include "TAttTreeProj.h"

namespace art {
   class TH1FTreeProj;
}

class art::TH1FTreeProj : public TH1F, public TAttTreeProj {
public:
   TH1FTreeProj();
   TH1FTreeProj(const char* name, const char *title, const char *cut = "");
   virtual ~TH1FTreeProj();

   TH1FTreeProj(const TH1FTreeProj& rhs);
   TH1FTreeProj& operator=(const TH1FTreeProj& rhs);

protected:

private:

   ClassDef(TH1FTreeProj,1) // Tree projection histogram
};

#endif // INCLUDE_GUARD_UUID_9041C331_C91A_46F6_94BC_7A22034D2591
