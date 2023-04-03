/**
 * @file   TArtemisCanvas.h
 * @brief  canvas for artemis object display
 *
 * @date   Created       : 2018-01-28 22:20:21 JST
 *         Last Modified : 2021-04-13 11:59:50 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2A71FDBB_E299_4510_8D78_08A10E7D2B1A
#define INCLUDE_GUARD_UUID_2A71FDBB_E299_4510_8D78_08A10E7D2B1A

#include "TNamed.h"
#include <TThreadPool.h>
#include "TSystem.h"
namespace art {
   class TArtemisCanvas;
}

class TCanvas;
class TVirtualPad;
class TPad;
class TPaveLabel;


class art::TArtemisCanvas : public TNamed {
public:
   class Control : public TThreadPoolTaskImp<art::TArtemisCanvas::Control, std::vector<TArtemisCanvas*>& >{
   public:
      bool runTask(std::vector<TArtemisCanvas*>& canvases) {
         while (1) {
            if (canvases.size() == 0) {
               return kTRUE;
            }
            for (std::vector<TArtemisCanvas*>::iterator it = canvases.begin(), itend = canvases.end();
                 it != itend; ++it) {
               (*it)->AutoUpdate();
            }
            gSystem->Sleep(100);
         }
      }
   };

public:
   typedef TThreadPool<art::TArtemisCanvas::Control,std::vector<art::TArtemisCanvas*>& > CanvasControl_t;
   
   TArtemisCanvas();
   TArtemisCanvas(const char* name, const char* title) {};
   virtual ~TArtemisCanvas();

   TArtemisCanvas(const TArtemisCanvas& rhs);
   TArtemisCanvas& operator=(const TArtemisCanvas& rhs);

   virtual void Divide(Int_t nx=1, Int_t ny=1, Float_t xmargin=0.01, Float_t ymargin=0.01, Int_t color=0); // *MENU*

   Bool_t AutoUpdate();

   void SetUpdateInterval(Int_t interval = 0);

   void SetWindowsSize(Int_t w, Int_t h) {}
   void SetWindowPosition(Int_t x, Int_t y) {}

   Int_t GetWxindowTopX() const { return fPosition[0]; }
   Int_t GetWxindowTopY() const { return fPosition[1]; }
   Int_t GetWindowWidth() const { return fSize[0]; }
   Int_t GetWindowHeight() const { return fSize[1]; }
   Int_t GetCurrentID() const { return fCurrentID; }
   Int_t GetNumPads() const { return fNumPads; }
   Int_t GetUpdateInterval() const { return fUpdateInterval; }
   Int_t GetLastUpdate() const { return fLastUpdate; }

   void Init();
   void Closed(); // signal catcher
   void MainPadClosed(); // signal catcher

   TVirtualPad* cd(Int_t subpadnumber = 1);

   virtual void Update(); // update 
   
protected:
   TCanvas *fCanvas; //! canvas
   TPad    *fMainPad; //! main pad
   Int_t fCurrentID;
   Int_t fNumPads;
   Int_t fSize[2];
   Int_t fDivision[2];
   Int_t fPosition[2];
   Int_t fUpdateInterval; // update interval (sec)
   Int_t fLastUpdate; //! last update time

   TPaveLabel *fTitleLabel; //!
   TPaveLabel *fDateLabel; //!

   void UpdateLabels(); // update date label

   static CanvasControl_t *fgCanvasControl;
   static std::vector<TArtemisCanvas*> fgCanvases;

private:

   ClassDef(TArtemisCanvas,1) // canvas for artemis objects
};

#endif // INCLUDE_GUARD_UUID_2A71FDBB_E299_4510_8D78_08A10E7D2B1A
