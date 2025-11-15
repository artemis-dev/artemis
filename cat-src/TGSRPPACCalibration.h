/**
 * @file   TGSRPPACCalibration.h
 * @brief  SRPPAC calibration GUI
 *
 * @date   Created      : 2014-04-03 09:56:00 JST
 *         Last Modified: 2019-08-30 18:25:40 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 OTA Shinsuke
 */

#ifndef INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38
#define INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38

#include <RQ_OBJECT.h>

namespace art {
   class TGSRPPACCalibration;
}

class TGWindow;
class TGMainFrame;
class TRootEmbeddedCanvas;
class TGNumberEntryField;
class TGDoubleHSlider;
class TGComboBox;
class TGPictureButton;

class TH1D;

class art::TGSRPPACCalibration {
   RQ_OBJECT("TGSRPPACCalibration")
public:
   TGSRPPACCalibration(const TGWindow *p, UInt_t w, UInt_t h, TObjArray* a);
   virtual ~TGSRPPACCalibration();

   void DoDraw();
   void CloseWindow();
   void Update();
   void UpdateByNE();
   void Save();
   void SaveSRPPAC();
   void Reset();
   void ChangeHist(Int_t id);
   void Prev();
   void Next();
   void SetRangeByXval();

   static void SetSRPPACParamDir(const char* dir){fgSRPPACParamDir = dir;};
   static void SetCalibPrmSubDir(const char* dir){fgCalibParamSubDir = dir;};

protected:
   void GenerateHist();
   void SaveToFile(const char*,Bool_t isAna = kFALSE);
   void UpdateComboBox();

private:
   TGMainFrame         *fMain;    //main frame
   TRootEmbeddedCanvas *fECanvas; //histograms are plotted here
   TGNumberEntryField  *fNEMin, *fNEMax;
   TGComboBox          *fComboSRPPAC, *fComboPlane;
   TGPictureButton     *fBXval, *fBReset, *fBSaveSingle, *fBSaveSRPPAC;

   Float_t     fXRange[2];        //x range of current histograms
   TH1D       *fHt;               //current drift time histogram
   TH1D       *fHtl;              //current histogram (integral)
   TObjArray  *fHists;            //histograms to be used for the calibration

   Int_t       fID;

   TString     fLastDir;
   static TString fgSRPPACParamDir;
   static TString fgCalibParamSubDir;

   void Build(const TGWindow *p, UInt_t w, UInt_t h);
   TString CurrentSRPPACName();
   TString CurrentPlaneName();

   TGSRPPACCalibration(const TGSRPPACCalibration& rhs);
   TGSRPPACCalibration& operator=(const TGSRPPACCalibration& rhs);

   ClassDef(TGSRPPACCalibration,0) // SRPPAC calibration GUI
};

#endif // INCLUDE_GUARD_UUID_03E5C476_3F51_4A69_A1C8_B205D8AF1F38
