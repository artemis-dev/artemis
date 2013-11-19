/* $Id:$ */
/**
 * @file   TRunInfo.cc
 * @date   Created : Nov 19, 2013 17:19:20 JST
 *   Last Modified : Nov 19, 2013 18:10:15 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRunInfo.h"

#include <TTimeStamp.h>
#include <TROOT.h>

using art::TRunInfo;

ClassImp(TRunInfo);

TRunInfo::TRunInfo()
   : fRunName(),fRunNumber(0),fStartTime(0),fStopTime(0),fHeader(),fEnder()
{
}

TRunInfo::TRunInfo(const char *name, const char *title)
   : TNamed(name, title),
     fRunName(),fRunNumber(0),fStartTime(0),fStopTime(0),fHeader(),fEnder()
{
}
TRunInfo::~TRunInfo()
{
}


void TRunInfo::Print(Option_t *) const
{
   TTimeStamp startTime(fStartTime-TTimeStamp::GetZoneOffset());
   TTimeStamp stopTime(fStopTime-TTimeStamp::GetZoneOffset());
   TString indent;
   for (Int_t i=0; i!= gROOT->GetDirLevel(); i++) {
      indent += " ";
   }
   printf("TRunInfo name='%s', title='%s'\n",GetName(),GetTitle());
   printf("%s Run   : %s%04d\n",indent.Data(),fRunName.Data(),fRunNumber);
   printf("%s Start : %s\n",indent.Data(),startTime.AsString("s"));
   printf("%s Stop  : %s\n",indent.Data(),stopTime.AsString("s"));
   printf("%s Header: %s\n",indent.Data(),fHeader.Data());
   printf("%s Ender : %s\n",indent.Data(),fEnder.Data());
}
