/* $Id:$ */
/**
 * @file   TCatThreadTask.h
 * @date   Created : Feb 19, 2012 10:19:19 JST
 *   Last Modified : Feb 19, 2012 10:41:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATTHREADTASK_H
#define TCATTHREADTASK_H

#include <TThreadPool.h>

class TCatThreadTask  
   : public TThreadPoolTaskImp<TCatThreadTask,Option_t*> {

public:
   TCatThreadTask(TCatProcessor *processor) {
      fProcessor = processor;
   }
   ~TCatThreadTask();

   bool runTask(Option_t *args) {
      fProcessor->Process(args);
   }
   TCatProcessor *fProcessor; //!
};
#endif // end of #ifdef TCATTHREADTASK_H
