/**
 * @file   TGateArrayInitializer.cc
 * @brief
 *
 * @date   Created:       2014-01-16 12:53:48
 *         Last Modified: 2014-06-19 15:14:02 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TGateArrayInitializer.h"

#include "TGateArray.h"

using art::TGateArrayInitializer;

ClassImp(art::TGateArrayInitializer);

// Default constructor
TGateArrayInitializer::TGateArrayInitializer() : fGates(NULL) {}

TGateArrayInitializer::~TGateArrayInitializer()
{
   delete fGates;
}

void TGateArrayInitializer::Init(TEventCollection *col)
{
   fGates = new TGateArray;
   fGates->Init();

   TList *infolist = fGates->GetInfoList();
   infolist->SetName("gateinfo");

   col->Add("gate",fGates,fOutputIsTransparent);
   col->AddInfo("gateinfo",fGates->GetInfoList(),fOutputIsTransparent);
}

void TGateArrayInitializer::Process()
{
   fGates->Reset();
}
