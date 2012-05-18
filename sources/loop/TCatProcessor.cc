/* $Id:$ */
/**
 * @file   TCatProcessor.cc
 * @date   Created : Feb 19, 2012 12:19:11 JST
 *   Last Modified : May 14, 2012 10:56:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatProcessor.h"

TCatProcessor::TCatProcessor()
   : fWidget(NULL), fInitialized(kFALSE)
{
   
}

TCatProcessor::~TCatProcessor()
{
}


void TCatProcessor::Clear(Option_t *)
{
   fInitialized = kFALSE;
}

void TCatProcessor::Initilized()
{
   fInitialized = kTRUE;
}


void TCatProcessor::SetName(const char *name)
{
   TNamed::SetTitle(ClassName());
   TNamed::SetName(name);
}
