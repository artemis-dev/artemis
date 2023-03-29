/**
 * @file   TGSRPPACConfig.h
 * @brief  SRPPAC config GUI
 *
 * @date   Created      : 2019-08-30 18:26:44 JST (ota)
 *         Last Modified: 2019-08-30 18:27:04 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 OTA Shinsuke
 */

#ifndef INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C
#define INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C

#include <RQ_OBJECT.h>
#include <TGFrame.h>

namespace art {
   class TGSRPPACConfig;
   class TGSRPPACConfigGroupFrame;
   class TSRPPACParameter;
}

class TGWindow;
class TGMainFrame;
class TGTextEntry;
class TGTextButton;
class TGCheckButton;
class TGComboBox;
class TGStatusBar;

class art::TGSRPPACConfig {
   RQ_OBJECT("TGSRPPACConfig")
public:
   TGSRPPACConfig();
   TGSRPPACConfig(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~TGSRPPACConfig();

   void CloseWindow();
   void SaveCalibConfig();
   void ResetCalibConfig();

protected:

private:
   void Build(const TGWindow *p, UInt_t w, UInt_t h);

   TGMainFrame *fMain;
   std::vector<void*> fComboArray;
   std::vector<const char*> fSRPPACList;
   std::vector<const char*> fConfigFileList;
   std::vector<TGSRPPACConfigGroupFrame*> fFrames;
   TGStatusBar      *fStatusBar;
   std::vector<TGComboBox*>  fCalibComboBoxes;

   ClassDef(TGSRPPACConfig,0) // SRPPAC config GUI
};

class art::TGSRPPACConfigGroupFrame : public TGGroupFrame {
public:
   TGSRPPACConfigGroupFrame(const TGWindow*, const char*);
   ~TGSRPPACConfigGroupFrame();
   void AddPlaneConfigEntries(const std::vector<const char*>&);
   void Save();
   void Reset();
   void LoadGeometryYaml();
   void Modified();
   Bool_t IsModified() const {return fIsModified;}

private:
   TString           fName;
   TGCompositeFrame *fContents;
   TGTextEntry      *fXEntry, *fYEntry, *fZEntry, *fAngleEntry;
   TGCheckButton    *fTurnedButton;
   TGComboBox       *fCBPlaneConfig;
   TGTextButton     *fSaveButton, *fResetButton;

   TSRPPACParameter *fGeometry;

   Bool_t          fIsModified;

   ClassDef(TGSRPPACConfigGroupFrame,0) // SRPPAC config GUI
};

#endif // INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C
