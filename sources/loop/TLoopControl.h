/* $Id:$ */
/**
 * @file   TLoopControl.h
 * @date   Created : Jul 10, 2013 17:10:33 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TLOOPCONTROL_H
#define TLOOPCONTROL_H

#include <TThreadPool.h>
#include <TLoop.h>

namespace art {
   class TLoopControl;
}

class art::TLoopControl  : public TThreadPoolTaskImp<art::TLoopControl,art::TLoop*> {

public:
   TLoopControl();
   ~TLoopControl();

   bool runTask(TLoop* loop);

   void Suspend() { fMode = kSuspend; }
private:
   enum {kResume, kSuspend} fMode;
      

};
#endif // end of #ifdef TLOOPCONTROL_H
