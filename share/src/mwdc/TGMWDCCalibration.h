/**
 * @file   TGMWDCCalibration.h
 * @brief  MWDC calibration GUI
 *
 * @date   Created       : 2014-04-03 09:56:00 JST
 *         Last Modified : 2014-07-04 13:24:22 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38
#define INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38

#include <RQ_OBJECT.h>

namespace art {
   class TGMWDCCalibration;
}

class TGWindow;
class TGMainFrame;
class TRootEmbeddedCanvas;
class TGNumberEntryField;
class TGDoubleHSlider;
class TGComboBox;
class TGPictureButton;

class TH1D;

class art::TGMWDCCalibration {
   RQ_OBJECT("TGMWDCCalibration")
public:
   TGMWDCCalibration(const TGWindow *p, UInt_t w, UInt_t h, TObjArray* a);
   virtual ~TGMWDCCalibration();

   void DoDraw();
   void CloseWindow();
   void Update();
   void UpdateByNE();
   void Save();
   void SaveMWDC();
   void Reset();
   void ChangeHist(Int_t id);
   void Prev();
   void Next();
   void SetRangeByXval();

   static void SetMWDCParamDir(const char* dir){fgMWDCParamDir = dir;};
   static void SetCalibPrmSubDir(const char* dir){fgCalibParamSubDir = dir;};

protected:
   void GenerateHist();
   void SaveToFile(const char*,Bool_t isAna = kFALSE);
   void UpdateComboBox();

private:
   TGMainFrame         *fMain;    //main frame
   TRootEmbeddedCanvas *fECanvas; //histograms are plotted here
   TGNumberEntryField  *fNEMin, *fNEMax;
   TGComboBox          *fComboMWDC, *fComboPlane;
   TGPictureButton     *fBXval, *fBReset, *fBSaveSingle, *fBSaveMWDC;

   Float_t     fXRange[2];        //x range of current histograms
   TH1D       *fHt;               //current drift time histogram
   TH1D       *fHtl;              //current histogram (integral)
   TObjArray  *fHists;            //histograms to be used for the calibration

   Int_t       fID;

   TString     fLastDir;
   static TString fgMWDCParamDir;
   static TString fgCalibParamSubDir;

   void Build(const TGWindow *p, UInt_t w, UInt_t h);
   TString CurrentMWDCName();
   TString CurrentPlaneName();

   TGMWDCCalibration(const TGMWDCCalibration& rhs);
   TGMWDCCalibration& operator=(const TGMWDCCalibration& rhs);

   ClassDef(TGMWDCCalibration,0) // MWDC calibration GUI
};

#endif // INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38
