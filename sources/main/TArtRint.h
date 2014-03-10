/* $Id:$ */
/**
 * @file   TArtRint.h
 * @date   Created : Feb 06, 2012 00:06:28 JST
 *   Last Modified : Mar 10, 2014 13:03:56 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TARTRINT_H
#define TARTRINT_H

#include <TRint.h>

static const char* fAppName = "artemis";
class TArtRint  : public TRint {

public:

   TArtRint(int* argc, char** argv, void* options = 0, int numOptions = 0, Bool_t noLogo = kFALSE);
   ~TArtRint();
private:
   TArtRint(const TArtRint&);

   virtual Long_t ProcessLine(const char* line, Bool_t sync = kFALSE, Int_t* error = 0);
   virtual void   Terminate(Int_t status);
};
#endif // end of #ifdef TARTRINT_H
