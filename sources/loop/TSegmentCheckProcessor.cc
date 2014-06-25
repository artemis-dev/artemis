/**
 * @file   TSegmentCheckProcessor.cc
 * @brief  segment check
 *
 * @date   Created       : 2014-05-18 14:16:52 JST
 *         Last Modified : Jun 22, 2014 21:46:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TSegmentCheckProcessor.h"
#include <TClonesArray.h>
#include <TDirectory.h>
#include <TSegmentedData.h>
#include <TSegmentInfo.h>
#include <TModuleType.h>
#include <TModuleInfo.h>
#include <TH2F.h>
#include <algorithm>
#include <TRawDataObject.h>
#include <TClass.h>

using art::TSegmentCheckProcessor;

ClassImp(TSegmentCheckProcessor)

TSegmentCheckProcessor::TSegmentCheckProcessor()
: fFolder(NULL), fHistDir(NULL), fSegmentList(NULL), fModuleList(NULL),
   fSegmentedData(NULL)
{
   StringVec_t defaultignore; defaultignore.push_back("");
   RegisterInputCollection("SegmentedDataName","name of the segmented data",
                           fSegmentedDataName,TString("segdata"),
                           &fSegmentedData,"art::TSegmentedData");
                           
   RegisterInputInfo("SegmentList","name of the segment list",
                     fSegmentListName,TString("seglist"),&fSegmentList,"TClonesArray","art::TSegmentInfo");
   RegisterInputInfo("ModuleList","name of the module list",
                     fModuleListName,TString("modlist"),&fModuleList,"TClonesArray","art::TModuleType");
   RegisterProcessorParameter("Ignore","ignore segment list",fIgnore,defaultignore);
}

TSegmentCheckProcessor::~TSegmentCheckProcessor()
{
   if (fHistDir) {
      fHistDir->Delete();
      delete fHistDir;
      fHistDir = NULL;
   }
   // input processors are deleted elsewhere
   fSegmentedData = NULL;
   fSegmentList = NULL;
   fModuleList = NULL;
   
}

TSegmentCheckProcessor::TSegmentCheckProcessor(const TSegmentCheckProcessor& rhs)
{
}

TSegmentCheckProcessor& TSegmentCheckProcessor::operator=(const TSegmentCheckProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TSegmentCheckProcessor::Init(TEventCollection *col)
{
   TDirectory *save = gDirectory;
   fHistDir = gDirectory->mkdir(GetName(),GetTitle());
   Int_t nSeg = (*fSegmentList)->GetEntriesFast();
   Info("Init","nSeg = %d",nSeg);
   
   for (Int_t iSeg=0; iSeg!=nSeg; iSeg++) {
      TSegmentInfo *seg = (TSegmentInfo*) (*fSegmentList)->At(iSeg);
      Int_t nMod = seg->GetNumModules();
      // skip if no module registered
      if (!nMod) continue;
      if (fIgnore.end() != std::find(fIgnore.begin(),fIgnore.end(),seg->GetName())) continue;

      std::pair<int, std::vector<TModuleInfo*> > segment;
      segment.first = seg->GetSegID();
      std::vector<TModuleInfo*> &modules = fSegments.insert(segment).first->second;
      
      TDirectory *segdir = fHistDir->mkdir(seg->GetName());
      segdir->cd();
      printf("%s nMod = %d\n",seg->GetName(),seg->GetNumModules());
      for (Int_t iMod = 0; iMod != nMod; iMod++) {
         TModuleInfo *mod = seg->GetModule(iMod);
         Int_t id = mod->GetID();
         printf("geo = %d\n",id);
         if (modules.size() < (unsigned int) id+1) modules.resize(id+1);
         modules.at(id) = mod;
         TModuleType *type = (TModuleType*)(*fModuleList)->FindObject(seg->GetModuleType());
         Int_t nCh = type->GetNch();
         Int_t nVal = type->GetNumValues();
         for (Int_t iVal = 0; iVal != nVal; iVal++) {
            const TAxis *axis = type->GetRange(iVal);
            TString name = TString::Format("%s_%d_%s",seg->GetName(),mod->GetID(),axis->GetName());
            mod->SetHist(iVal,new TH2F(name,name,nCh,-0.5,nCh-0.5,axis->GetNbins(),axis->GetXmin(),axis->GetXmax()));
         }
      }
   }
   save->cd();

   for (std::vector<TString>::iterator it = fIgnore.begin();
        it != fIgnore.end(); it++) {
      printf("%s will be ignored\n",(*it).Data());
   }
}

void TSegmentCheckProcessor::Process()
{
   std::map<int, std::vector<TModuleInfo*> >::iterator it;
   for (it = fSegments.begin(); it != fSegments.end(); it++) {
      Int_t segid = it->first;
      TObjArray *arr = (*fSegmentedData)->FindSegmentByID(segid);
      if (!arr) {
         Warning("Process","No segment having segid = %d",segid);
         Warning("Process"," Add this segid to Ignore if this semgment is not valid temporarily");
         SetStopLoop();
         return;
      }
      Int_t nHit = arr->GetEntriesFast();
//      printf("segid = %d, nHit = %d\n",segid,nHit);
      std::vector<TModuleInfo*> &modules = it->second;
//      printf("size = %u\n",modules.size());
      for (Int_t iHit = 0; iHit != nHit; iHit++) {
         TRawDataObject *data = (TRawDataObject*) arr->UncheckedAt(iHit);
         Int_t geo = data->GetGeo();
         Int_t ch = data->GetCh();
         Int_t nVal = data->GetNumValues();
//         printf("Data = %s geo = %d ch = %d nVal = %d\n",data->IsA()->GetName(),geo,ch,nVal);
         for (Int_t iVal = 0; iVal != nVal; iVal++) {
            if (modules.size() > geo && modules[geo] != NULL) {
               TH2F* hist = (TH2F*)  modules[geo]->GetHist(iVal);
//            printf("%p\n",hist);
               hist->Fill(ch,data->GetValue(iVal));
            }
         }
      }
   }
}
