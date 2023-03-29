/**
 * @file   TEventDisplayProcessorHelper.h
 * @brief
 *   when process is called the canvas and its child pads are updated and saved if mode is set to kDrawSave
 * mode
 *   0 quiet         : do nothing
 *   1 draw only     : update canvas and do nothing
 *   2 draw and save : update canvas and save canvas
 *
 * @date   Created       : 2016-07-13 15:34:25 JST
 *         Last Modified : 2016-07-15 08:11:31 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2343E725_219F_4DA1_85A4_25D3BA7D1BB6
#define INCLUDE_GUARD_UUID_2343E725_219F_4DA1_85A4_25D3BA7D1BB6

#include "TNamed.h"
#include <TString.h>
#include <IProcessorHelper.h>

namespace art {
   class TEventDisplayProcessorHelper;
   class TProcessor;
   class TEventCollection;
   class TEventHeader;
}

class TCanvas;
class TPaveLabel;
class TPad;

class art::TEventDisplayProcessorHelper : public TNamed, public IProcessorHelper {
public:
   typedef enum { kQuiet, kDrawOnly, kDrawSave } DisplayMode_t;
   TEventDisplayProcessorHelper();
   TEventDisplayProcessorHelper(const char *name, const char *title);
   virtual ~TEventDisplayProcessorHelper();

   TEventDisplayProcessorHelper(const TEventDisplayProcessorHelper& rhs);
   TEventDisplayProcessorHelper& operator=(const TEventDisplayProcessorHelper& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Register(TProcessor *proc);
   virtual void Process();
   virtual TPad *GetMainPad() const { return fMainPad; }
   virtual void SetWindowSize(UInt_t ww, UInt_t wh);
   virtual Bool_t Displays() { return fDisplayMode != kQuiet; }

   virtual void SetWaitTime(Int_t waitTime) { fWaitTime = waitTime; }
   virtual Int_t GetWaitTime() const { return fWaitTime; }
   virtual void SetDisplayMode(DisplayMode_t mode) { fDisplayMode = mode; }
   virtual Int_t GetDisplayMode() const { return fDisplayMode; }


protected:
   TCanvas *fCanvas; //!
   TPad    *fMainPad; //
   TPaveLabel *fTitleLabel; //!
   TPaveLabel *fDateLabel; //!
   TPaveLabel *fEventInfoLabel;//!
   Int_t    fWaitTime;
   Int_t    fDisplayMode; // draw flag 0 skip, 1 draw, 2 draw and save

   TString        fNameEventHeader; //
   TEventHeader **fEventHeader; //!

   TString fDirNameFmt;
   TString fDirName;
   TString fFileNameFmt;

   Bool_t kIsInitalized;


private:

   ClassDef(TEventDisplayProcessorHelper,1) //
};

#endif // INCLUDE_GUARD_UUID_2343E725_219F_4DA1_85A4_25D3BA7D1BB6
