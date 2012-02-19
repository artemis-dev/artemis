/* $Id:$ */
/**
 * @file   TCatObject.h
 * @date   Created : Jan 31, 2012 11:31:34 JST
 *   Last Modified : Feb 11, 2012 17:59:46 JST
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
   typedef enum { kID } ESortType;
   typedef enum { kASC, kDESC } ESortOrder;
public:
   TCatObject();
   ~TCatObject();

   // static void SetSortType(const Int_t &type) { fSortType = type; }

   UInt_t GetOptLevel() const { return fOptLevel; }
   void   SetOptLevel(UInt_t level) { fOptLevel = level; }

   UInt_t Operated() { fOptLevel++; return fOptLevel; }

   UInt_t GetCategoryID() const { return fCategoryID; }
   void   SetCategoryID(UInt_t id) { fCategoryID = id; }

   UInt_t GetDetectorID() const { return fDetectorID; }
   void   SetDetectorID(UInt_t id) { fDetectorID = id; }

   virtual Int_t Compare(const TObject *obj) const;
   virtual Bool_t IsSortable() const { return kTRUE;}

   static void SetSortType(const Int_t &type) { fSortType = type; }
   static void SetSortOrder(const Int_t &order) { fSortOrder = order; }

protected:
   static Int_t fSortType;
   static Int_t fSortOrder;
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
