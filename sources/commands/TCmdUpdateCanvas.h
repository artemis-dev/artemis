/**
 * @file   TCmdUpdateCanvas.h
 * @brief  update canvas including child pads
 *
 * @date   Created       : 2014-06-26 13:52:16 JST
 *         Last Modified : Jun 26, 2014 17:09:10 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_FBBCE33E_0780_4FF8_90FC_6B67C910A472
#define INCLUDE_GUARD_UUID_FBBCE33E_0780_4FF8_90FC_6B67C910A472

#include "TCatCmd.h"
#include <list>

namespace art {
   class TCmdUpdateCanvas;
}

class TVirtualPad;
class TThread;

class art::TCmdUpdateCanvas : public TCatCmd {
public:
   static const Int_t kDefaultInterval;
   typedef struct UpdateArg {
      TVirtualPad *fPad;
      Int_t        fInterval;
      TThread     *fThread;
      Bool_t      *fUpdate;
   } UpdateArg_t;
      
   TCmdUpdateCanvas();
   virtual ~TCmdUpdateCanvas();

   TCmdUpdateCanvas(const TCmdUpdateCanvas& rhs);
   TCmdUpdateCanvas& operator=(const TCmdUpdateCanvas& rhs);

   virtual Long_t Cmd(vector<TString> tokens);
   virtual Long_t Run(TVirtualPad *pad, Int_t interval = 2000);
   virtual void Help();
   
protected:
   std::list<UpdateArg_t> fUpdateArgs;

   static void* UpdateCanvasThread(void *updatearg);
   static void UpdateCanvasImpl(TVirtualPad *pad);
   

private:

   ClassDef(TCmdUpdateCanvas,1) // update canvas including child pads
};

#endif // INCLUDE_GUARD_UUID_FBBCE33E_0780_4FF8_90FC_6B67C910A472
