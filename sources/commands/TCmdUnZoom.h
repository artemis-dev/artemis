/**
 * @file   TCmdUnZoom.h
 * @brief  unzoom current histogram
 *
 * @date   Created       : 2015-12-21 13:26:40 JST
 *         Last Modified : 2015-12-22 10:43:55 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_1B782C4D_E906_4693_ADB0_A99379B6A064
#define INCLUDE_GUARD_UUID_1B782C4D_E906_4693_ADB0_A99379B6A064

#include "TCatCmd.h"

namespace art {
   class TCmdUnZoom;
}

class TH1;

class art::TCmdUnZoom : public TCatCmd {
public:
   TCmdUnZoom();
   virtual ~TCmdUnZoom();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TH1 *h, Option_t *opt) const;
   void   Help();

private:
   TCmdUnZoom(const TCmdUnZoom&);            // undefined
   TCmdUnZoom& operator=(const TCmdUnZoom&); // undefined

   ClassDef(TCmdUnZoom,1) // unzoom current histogram
};

#endif // INCLUDE_GUARD_UUID_1B782C4D_E906_4693_ADB0_A99379B6A064
