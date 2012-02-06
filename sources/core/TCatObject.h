/* $Id:$ */
/**
 * @file   TCatObject.h
 * @date   Created : Jan 31, 2012 11:31:34 JST
 *   Last Modified : Feb 04, 2012 22:54:38 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TARTCATOBJECT_H
#define TARTCATOBJECT_H

#include "TNamed.h"

class TCatObject  : public TNamed {

public:
   TCatObject();
   ~TCatObject();

   UInt_t GetOptLevel() const { return fOptLevel; }
   void   SetOptLevel(UInt_t level) { fOptLevel = level; }

   UInt_t Operated() { fOptLevel++; return fOptLevel; }

   UInt_t GetCategoryID() const { return fCategoryID; }
   void   SetCategoryID(UInt_t id) { fCategoryID = id; }

   UInt_t GetDetectorID() const { return fDetectorID; }
   void   SetDetectorID(UInt_t id) { fDetectorID = id; }

protected:
   UInt_t fCategoryID;
   UInt_t fDetectorID; // id of this object

   /**
    * operation level
    * raw data must have operation level = 0.
    * after some operation done, operation level should be incremented.
    */
   UInt_t fOptLevel; 

   ClassDef(TCatObject,1);
};
#endif // end of #ifdef TARTCATOBJECT_H
