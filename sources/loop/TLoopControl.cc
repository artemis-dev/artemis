/* $Id:$ */
/**
 * @file   TLoopControl.cc
 * @date   Created : Jul 10, 2013 18:10:21 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TLoopControl.h"

art::TLoopControl::TLoopControl()
   : fMode(kResume) {
}
art::TLoopControl::~TLoopControl()
{
}

bool art::TLoopControl::runTask(TLoop* loop)
{
   switch (fMode) {
   case kResume:
      return loop->Resume();
      break;
   case kSuspend:
      loop->GetCondition()->Set(TLoop::kStopLoop);
      return kTRUE;
      break;
   } 
}
