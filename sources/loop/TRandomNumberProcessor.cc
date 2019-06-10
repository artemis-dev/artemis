/* $Id:$ */
/**
 * @file   TRandomNumberProcessor.cc
 * @date   Created : Jul 11, 2013 22:11:40 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRandomNumberProcessor.h"

art::TRandomNumberProcessor::TRandomNumberProcessor()
{
   RegisterProcessorParameter("InputColName","the name of input collection",
                              fInputColName,TString("random"));
   
   RegisterProcessorParameter("OutputColName","the name of output collection",
                              fOutputColName,TString("randommod"));
   
}
art::TRandomNumberProcessor::~TRandomNumberProcessor()
{
}

void art::TRandomNumberProcessor::Init(TEventCollection *col)
{
   fOutData = new TSimpleData;
   fData = (TSimpleData**) col->Get(fInputColName)->GetObjectRef();
   col->Add(fOutputColName,fOutData,fOutputIsTransparent);
}


void art::TRandomNumberProcessor::Process()
{
   fOutData->SetValue(10.*(*fData)->GetValue());
}
