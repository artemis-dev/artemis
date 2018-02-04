/* $Id:$ */
/**
 * @file   TRandomNumberProcessor.h
 * @date   Created : Jul 11, 2013 22:11:10 JST
 *   Last Modified : 2018-02-04 11:04:43 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRANDOMNUMBERPROCESSOR_H
#define TRANDOMNUMBERPROCESSOR_H

#include <TProcessor.h>
#include <TSimpleData.h>


namespace art {
   class TRandomNumberProcessor;
}

class art::TRandomNumberProcessor  : public TProcessor {

public:
   TRandomNumberProcessor();
   ~TRandomNumberProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   
private:
   TSimpleData **fData; //!
   TString fInputColName;
   TString fOutputColName;
   TSimpleData *fOutData;


   ClassDef(TRandomNumberProcessor,1);
};
#endif // end of #ifdef TRANDOMNUMBERPROCESSOR_H
