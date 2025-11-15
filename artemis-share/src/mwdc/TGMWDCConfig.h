/**
 * @file   TGMWDCConfig.h
 * @brief  MWDC config GUI
 *
 * @date   Created       : 2014-07-04 13:33:09 JST
 *         Last Modified : 2014-07-11 11:16:55 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C
#define INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C

#include <RQ_OBJECT.h>
#include <TGFrame.h>

namespace art {
   class TGMWDCConfig;
   class TGMWDCConfigGroupFrame;
   class TMWDCParameter;
}

class TGWindow;
class TGMainFrame;
class TGTextEntry;
class TGTextButton;
class TGCheckButton;
class TGComboBox;
class TGStatusBar;

class art::TGMWDCConfig {
   RQ_OBJECT("TGMWDCConfig")
public:
   TGMWDCConfig();
   TGMWDCConfig(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~TGMWDCConfig();

   void CloseWindow();
   void SaveCalibConfig();
   void ResetCalibConfig();

protected:

private:
   void Build(const TGWindow *p, UInt_t w, UInt_t h);

   TGMainFrame *fMain;
   std::vector<void*> fComboArray;
   std::vector<const char*> fMWDCList;
   std::vector<const char*> fConfigFileList;
   std::vector<TGMWDCConfigGroupFrame*> fFrames;
   TGStatusBar      *fStatusBar;
   std::vector<TGComboBox*>  fCalibComboBoxes;

   ClassDef(TGMWDCConfig,0) // MWDC config GUI
};

class art::TGMWDCConfigGroupFrame : public TGGroupFrame {
public:
   TGMWDCConfigGroupFrame(const TGWindow*, const char*);
   ~TGMWDCConfigGroupFrame();
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

   TMWDCParameter *fGeometry;

   Bool_t          fIsModified;

   ClassDef(TGMWDCConfigGroupFrame,0) // MWDC config GUI
};

#endif // INCLUDE_GUARD_UUID_8FE4A6A3_C408_41AC_BD21_5F31C1A77A0C
