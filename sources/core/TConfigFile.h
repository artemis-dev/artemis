/* $Id:$ */
/**
 * @file   TConfigFile.h
 * @date   Created : Jul 17, 2013 05:17:45 JST
 *   Last Modified : 2019-04-25 22:40:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCONFIGFILE_H
#define TCONFIGFILE_H

#include <TString.h>
#include <TList.h>
#include <fstream>
#include "TObjArray.h"

namespace art {
   class TConfigFile;
}



class art::TConfigFile  {

public:
   TConfigFile(const char* filename,
               const char *comment = "#",
               const char *delim = ", \t",
               const char *eol = ";");
   ~TConfigFile();
   

   TString GetNextToken();
private:
   std::ifstream  fFile;
   TString   fComment;
   TString   fDelim;
   TString   fEOL;
   TObjArray *fTokens; //->
   Int_t     fIdx;

   ClassDef(TConfigFile,1);
};
#endif // end of #ifdef TCONFIGFILE_H
