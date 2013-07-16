/* $Id:$ */
/**
 * @file   TConfigFile.cc
 * @date   Created : Jul 17, 2013 05:17:55 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TConfigFile.h"

#include <TObjArray.h>
#include <TObjString.h>

ClassImp(art::TConfigFile);

art::TConfigFile::TConfigFile(const char *filename,
                              const char *comment,
                              const char *delim,
                              const char *eol)
   : fComment(comment),fDelim(delim),fEOL(eol),fTokens(NULL)
{
   fFile.open(filename);
   if (!fFile) {
      perror("file not exist");
   }
   fTokens = new TList;
   fEOL.Resize(1);
}
art::TConfigFile::~TConfigFile()
{
   if (fFile) fFile.close();
   if (fTokens) delete fTokens;
}


TString art::TConfigFile::GetNextToken()
{
   if (!fTokens->GetEntries()) {
      TString line;
      while (1) { 
         line.ReadLine(fFile);
         if (!fFile) break;
         for (Int_t i=0; i!=fComment.Length(); i++) {
            TString c(fComment.Data()[i]);
            if (line.BeginsWith(c.Data())) {
               line.Resize(0);
               break;
            }

         }
         if (line.Length()) break;
      }
      // if EOF, return blank
      if (!fFile) return TString("");

      // remove inline comment
      if (line.Index(fEOL) != -1) {
         line.Replace(line.Index(fEOL),line.Sizeof()-line.Index(fEOL),"");
      }

      TObjArray *arr = line.Tokenize(fDelim);
      for (Int_t i=0; i!=arr->GetEntriesFast();i++) {
         fTokens->AddLast(arr->At(i)->Clone());
      }
      delete arr;
   }
   if (!fTokens->GetEntries()) return TString("");

   TObjString *strobj = (TObjString*) fTokens->First();
   TString str =strobj->String();
   fTokens->RemoveFirst();
   delete strobj;
   return str;
}
