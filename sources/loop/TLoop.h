
/* $Id:$ */
/**
 * @file   TLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : 2019-04-26 10:38:42 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TLOOP_H
#define ART_TLOOP_H


#include <list>

#include "TConditionBit.h"
#include "TEventCollection.h"
#include "TAnalysisInfo.h"

namespace art {
   class TLoop;
   class TProcessor;
   class TAnalysisInfo;
   class IEventStore;
};

namespace YAML {
   class Node;
}



class art::TLoop : public TObject {

public:
   /*
    * enumeration for conditions 
    */
   enum EProc {kRunning, kBeginOfRun, kEndOfRun, kStopEvent, kStopLoop};
   static const char* kConditionName;
   static const char* kAnalysisInfoName; 


   TLoop();
   ~TLoop();


   Bool_t Init();
   Bool_t Resume();
   Bool_t Terminate();
   void SetID(Int_t id) { fID = id;}
   Int_t GetID() { return fID; }

   virtual void        Clear(Option_t * /*option*/ ="") { }

   Bool_t Load(const char* dirname, const char* basename, std::list<Long_t> *loaded, 
               std::map<std::string,std::string>* replace = NULL);

   virtual Bool_t LoadYAMLNode(const YAML::Node& node, std::list<Long_t> *loaded);

   
   TConditionBit *GetCondition() { return fCondition; }

   Bool_t IsRunning() { return fCondition->IsSet(kRunning); }

private:
   TConditionBit *fCondition;
   
   TEventCollection   *fEventCollection;
   std::list<TProcessor*>   fProcessors; //!
   Int_t fID;

   TString fBaseDir; //! base directory
   TAnalysisInfo *fAnalysisInfo; //->  analysis information
   IEventStore  *fEventStore; //! Event store to get analysis information

   ClassDef(TLoop,1);
};


#endif // end of #ifdef TLOOP_H
