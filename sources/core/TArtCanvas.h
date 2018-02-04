/**
 * @file   TArtCanvas.h
 * @brief  artemis canvas class
 *
 * @date   Created       : 2017-06-25 12:48:58 JST
 *         Last Modified : 2017-06-29 08:59:07 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_CEE4E937_2AF8_4156_884F_CE442B27414F
#define INCLUDE_GUARD_UUID_CEE4E937_2AF8_4156_884F_CE442B27414F

#include "TCanvas.h"

namespace art {
   class TArtCanvas;
}

class TPaveLabel;

class art::TArtCanvas : public TCanvas {
public:
   TArtCanvas(Bool_t build = kTRUE);
   TArtCanvas(const char* name, const char *title ="", Int_t form = 1);
   TArtCanvas(const char* name, const char *title, Int_t ww, Int_t wh);
   TArtCanvas(const char *name, const char *title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh);
   TArtCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
   virtual ~TArtCanvas();

   TArtCanvas(const TArtCanvas& rhs);
   TArtCanvas& operator=(const TArtCanvas& rhs);

   // create new main pad
   virtual void ConstructMainPad();
   virtual void SetTitleLabel(const char* title);
   virtual void SetCommentLabel(const char* comment);
   virtual void UpdateDateLabel();

   // override methods of TCanvas
   virtual void Clear(Option_t *option);
   virtual void Divide(Int_t nx = 1, Int_t ny = 1, Float_t xmargin = 0.01, Float_t ymargin = 0.01, Int_t color = 0);
#if 0
   virtual void Update();
#endif
protected:
   TVirtualPad *fMainPad; //!

   
   TPaveLabel *fTitleLabel; //!
   TPaveLabel *fDateLabel; //!
   TPaveLabel *fCommentLabel; //!
   

private:

   ClassDef(TArtCanvas,1) // artemis canvas class
};

#endif // INCLUDE_GUARD_UUID_CEE4E937_2AF8_4156_884F_CE442B27414F
