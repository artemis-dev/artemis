/* $Id:$ */
/**
 * @file   TArtAtomicMassTable.h
 * @date   Created : Aug 04, 2011 15:04:58 JST
 *   Last Modified : Aug 05, 2011 15:11:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTATOMICMASSTABLE_H
#define TARTATOMICMASSTABLE_H

#include <TROOT.h>


class TArtAtomicMassTable : public TObject {
public:
   TArtAtomicMassTable();
   ~TArtAtomicMassTable();

   /** 
    * set mass table from mass-table file
    *
    * @param filename atomic mass table file
    * @param firstLine the first line where the data begins at
    *
    */
   void SetMassTable(const char *filename, Int_t firstLine);
   
   // get atomic mass by atomic number and mass number
   Double_t GetAtomicMass(Int_t z, Int_t a);

   // get nucleus mass by atomic number and mass number
   Double_t GetNucleusMass(Int_t z, Int_t a);

   static const Int_t kNumZ = 200;
   static const Int_t kNumA = 300;
   static const TString kEnvName;
private:
   TString  fTableName;
   Double_t fMass[kNumZ][kNumA];
   Bool_t   fIsEvaluated[kNumZ][kNumA];

   ClassDef(TArtAtomicMassTable,1);
};

extern TArtAtomicMassTable *gAtomicMassTable;

#endif // end of #ifdef TARTATOMICMASSTABLE_H
