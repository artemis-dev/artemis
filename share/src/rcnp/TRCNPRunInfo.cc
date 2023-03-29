/* $Id:$ */
/**
 * @file   TRCNPRunInfo.cc
 * @date   Created : Nov 19, 2013 17:19:20 JST
 *   Last Modified : Jun 22, 2014 17:14:09 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRCNPRunInfo.h"

#include <TTimeStamp.h>
#include <TROOT.h>

using art::TRCNPRunInfo;

ClassImp(TRCNPRunInfo);

TRCNPRunInfo::TRCNPRunInfo()
   : fRunName(),fRunNumber(0),fStartTime(0),fStopTime(0),fHeader(),fEnder(), fEventNumber(0), fTotalSize(0), fAnalyzedSize(0)
{
}

TRCNPRunInfo::TRCNPRunInfo(const char *name, const char *title)
   : TNamed(name, title),
     fRunName(),fRunNumber(0),fStartTime(0),fStopTime(0),fHeader(),fEnder(), fEventNumber(0), fTotalSize(0), fAnalyzedSize(0)
{
}
TRCNPRunInfo::~TRCNPRunInfo()
{
}


void TRCNPRunInfo::Print(Option_t *) const
{
   TTimeStamp startTime(fStartTime-TTimeStamp::GetZoneOffset());
   TTimeStamp stopTime(fStopTime-TTimeStamp::GetZoneOffset());
   TString indent;
   for (Int_t i=0; i!= gROOT->GetDirLevel(); i++) {
      indent += " ";
   }
   printf("TRCNPRunInfo name='%s', title='%s'\n",GetName(),GetTitle());
   printf("%s Size  : %.2f MB\n",indent.Data(),(fTotalSize/1024./1024.));
   printf("%s Run   : %s%04d\n",indent.Data(),fRunName.Data(),fRunNumber);
   printf("%s Start : %s\n",indent.Data(),startTime.AsString("s"));
   printf("%s Stop  : %s\n",indent.Data(),stopTime.AsString("s"));
   printf("%s Header: %s\n",indent.Data(),fHeader.Data());
   printf("%s Ender : %s\n",indent.Data(),fEnder.Data());
   printf("%s %d events analyzed\n",indent.Data(),fEventNumber);
   printf("%s %.2f MB analyzed \n",indent.Data(),(fAnalyzedSize/1024./1024.));
}
