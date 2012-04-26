/* $Id:$ */
/**
 * @file   TCatEventCollection.h
 * @date   Created : Apr 26, 2012 23:26:19 JST
 *   Last Modified : Apr 26, 2012 23:48:26 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTCOLLECTION_H
#define TCATEVENTCOLLECTION_H

#include <TROOT.h>

class TCatEventCollection  {

public:
   TCatEventCollection();

   // for shared memory 
   TCatEventCollection(Int_t shmid); 
   // for file datasource
   TCatEventCollection(const char* filename);

   ~TCatEventCollection();
};
#endif // end of #ifdef TCATEVENTCOLLECTION_H
