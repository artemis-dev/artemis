/* $Id:$ */
/**
 * @file   TCatLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : Apr 27, 2012 00:20:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoop.h"

#include <fstream>
using namespace std;

TCatLoop::TCatLoop()
{
   fEvtNum = 0;
   fIsOnline = kTRUE;
   fOutput = "temp.root";
}
TCatLoop::~TCatLoop()
{
}

Bool_t TCatLoop::AddInputFile(const char *inputfile)
{
   ifstream fin;
   fin.open(inputfile);
   if (!fin) {
      printf("file not found : %s\n",inputfile);
      return kFALSE;
   }
   fIsOnline = kFALSE;
   fInputs.push_back(TString(inputfile));
   return kTRUE;
}

Bool_t TCatLoop::SetOutput(const char *outputfile)
{
   ifstream fin;
   fin.open(outputfile);
   if (!fin) {
      printf("file already exist : %s\n",outputfile);
      return kFALSE;
   }
   fOutput = outputfile;
   return kTRUE;
}

Bool_t TCatLoop::Resume()
{
   fIsSuspend = kFALSE;
   if (!Open()) return kFALSE;
   while (fEvtNum < 10) {
      if (fIsSuspend) break;
      sleep(1);
      fEvtNum++;
      printf("fEvtNum = %d\n",fEvtNum);
   }
   return kTRUE;
}

Bool_t TCatLoop::Suspend()
{
   fIsSuspend = kTRUE;
   return kTRUE;
}

Bool_t TCatLoop::Terminate()
{
   fEvtNum = 0;
   return kTRUE;
}


Bool_t TCatLoop::Open(Int_t shmid)
{
   if (fIsOnline) {
      fEventCollection = new TCatEventCollection(shmid);
      return kTRUE;
   }
   if (fInputs.size() == 0) {
      return kFALSE;
   }
   TString filename = fInputs.front();
   fInputs.pop_front();
   fEventCollection = new TCatEventCollection(filename);
   return kTRUE;
}

