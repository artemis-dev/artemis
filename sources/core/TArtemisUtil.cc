/**
 * @file   TArtemisUtil.cc
 * @brief
 *
 * @date   Created       : 2018-07-26 16:33:05 JST
 *         Last Modified : 2018-07-26 21:48:22 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TArtemisUtil.h"
#include <TString.h>
#include <TSystem.h>

using namespace art;


Bool_t Util::PrepareDirectoryFor(const char *filename)
{
   TString pwd = gSystem->pwd();
   TString target = gSystem->DirName(filename);
   if (gSystem->ChangeDirectory(target)) {
      // target directory exists
      gSystem->ChangeDirectory(pwd);
      return kTRUE;
   } else {
      return ( 0 == gSystem->mkdir(target,kTRUE) );
   }
}



