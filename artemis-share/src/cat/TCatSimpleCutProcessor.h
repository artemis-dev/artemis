/* $Id:$ */
/**
 * @file   TCatSimpleCutProcessor.h
 * @date   Created : Sep 27, 2012 10:27:48 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  Cut for TCatSimpleData
 *    Copyright (C)2012
 */
#ifndef TCATSIMPLECUTPROCESSOR_H
#define TCATSIMPLECUTPROCESSOR_H

#include <TCatProcessor.h>
#include <TCatObject.h>
#include <map>
using namespace std;

class TClass;
class TCatEventCollection;
class TCatSimpleData;

class TCatSimpleCutProcessor  : public TCatProcessor {

public:
   TCatSimpleCutProcessor();
   virtual ~TCatSimpleCutProcessor();

   virtual void Init(TCatEventCollection *);
   virtual void Process();

   
protected:
   virtual const char* GetInputClass() { return "TCatSimpleData"; }
   virtual Bool_t  IsValid(const TCatSimpleData& data) = 0;

   
   StringVec_t fInputColName;
   TString fOutputColName;
   TString fPrmFileName;

   TClonesArray *fInputArray;
   TClonesArray *fOutputArray;
   
   vector<Double_t> fGateMin;
   vector<Double_t> fGateMax;
   

   ClassDef(TCatSimpleCutProcessor,1);
};
#endif // end of #ifdef TCATSIMPLECUTPROCESSOR_H
