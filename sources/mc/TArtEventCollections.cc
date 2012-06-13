/* $Id:$ */
/**
 * @file   TArtEventCollections.cc
 * @date   Created : Sep 09, 2011 18:09:18 JST
 *   Last Modified : Sep 11, 2011 13:31:08 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtEventCollections.h"

ClassImp(TArtEventCollections);

TArtEventCollections::TArtEventCollections() {
      fObjects = new TList;
      fOutputObjects = new TList;
      fObjects->SetName("EventCollectionAll");
      fOutputObjects->SetName("EventCollection");
}

TArtEventCollections::~TArtEventCollections() {
      delete fObjects;
      delete fOutputObjects;

}
