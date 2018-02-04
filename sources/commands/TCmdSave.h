/* $Id:$ */
/**
 * @file   TCmdSave.h
 * @date   Created : Oct 01, 2012 13:01:07 JST
 *   Last Modified : May 03, 2015 17:43:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 * TCmdSave
 * 
 * save [filename]
 *
 * Save canvas drawn by using ht commands.
 *
 * If no filename is given, AutoName flag is set and the file
 * format(s) is given, then the file is automatically named using
 * epoch.
 * In order to enable this feature, please call the functions as indicated below.
 *
   TCmdSave * cmdsave = TCmdSave::Instance();
   // set default directory
   cmdsave->SetDefaultDirectory("figs");
   // set flag to add the date directory to the default directory
   // The directory figs/20150503 is default directory on May 3rd, 2015.
   cmdsave->SetAddDateDir(kTRUE);
   // Set flag for automatically naming
   cmdsave->SetAutoName(kTRUE);
   // Add some files formats.
   // The canvas will be saved as all of the indicated file formats.
   cmdsave->AddFormat("png");
   cmdsave->AddFormat("root");
   cmdsave->AddFormat("pdf");
 * 
 * 
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDSAVE_H
#define TCATCMDSAVE_H

#include <TCatCmd.h>

namespace art {
  class TCmdSave;
}

class art::TCmdSave  : public TCatCmd {

public:
   
   TCmdSave();
   ~TCmdSave();

   static TCmdSave* Instance();
   virtual Long_t Cmd(vector<TString> tokens);
   virtual Long_t Run(const char* filename = "");

   virtual void SetAddDateDir(Bool_t add = kTRUE) { fAddDateDir = add; }
   virtual Bool_t GetAddDateDir() { return fAddDateDir; }
   
   virtual void SetHasDateFormat(Bool_t has = kTRUE) { fHasDateFormat = has; }
   virtual Bool_t HasDateFormat() { return fHasDateFormat; }
   virtual void SetDefaultDirectory(const char* dir) { fDefaultDirectory = dir; }
   virtual const char* GetDefaultDirectory() const { return fDefaultDirectory.Data(); }

   virtual void SetAutoName(Bool_t autosave = kTRUE) { fAutoName = autosave; }
   virtual Bool_t GetAutoName() { return fAutoName; }

   virtual void AddFormat(const char* suffix, Bool_t isPrintFormat = kFALSE);

   virtual const char* GetPrintFileName() {
     return fLastPrintFile.IsNull() ? NULL : fLastPrintFile.Data();
   }

protected:
   TString fDefaultDirectory; // Directory to save
   Bool_t fAddDateDir; // flag for adding date dir to default directory
   Bool_t  fHasDateFormat; // 1 if the default directory has date format
   Bool_t  fAutoName; // flag for automatic naming
   std::vector<TString> fFormat; // list of file format
   Int_t fPrintFormat; // file format for printing
   TString fLastPrintFile; // last filename for printing

   ClassDef(TCmdSave,1); // save current canvas

};
#endif // end of #ifdef TCATCMDSAVE_H
