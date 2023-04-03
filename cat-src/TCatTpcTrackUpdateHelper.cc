/**
 * @file   TCatTpcTrackUpdateHelper.cc
 * @brief  track update helper
 *
 * @date   Created       : 2018-02-25 16:15:51 JST
 *         Last Modified : 2020-05-22 14:49:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TCatTpcTrackUpdateHelper.h"
#include <TProcessor.h>
#include <TClonesArray.h>
#include <TClass.h>
#include <TCatTpcTrack.h>

using art::TCatTpcTrackUpdateHelper;

ClassImp(TCatTpcTrackUpdateHelper)

// helper
namespace {
   using art::TCatTpcTrackUpdateHelper;
   const Int_t numUpdateTypes = 4;
   const TString typeName[numUpdateTypes] = {
      "None", "ExtrapolateToOrigin", "OffsetToOrigin", "ExtrapolateToVertex"
   };
   const TCatTpcTrackUpdateHelper::UpdateType_t types[numUpdateTypes] = {
      TCatTpcTrackUpdateHelper::kNone,
      TCatTpcTrackUpdateHelper::kExtrapolateToOrigin, 
      TCatTpcTrackUpdateHelper::kOffsetToOrigin,
      TCatTpcTrackUpdateHelper::kExtrapolateToVertex
   };
}



// static members
const Int_t TCatTpcTrackUpdateHelper::kNumUpdateTypes = numUpdateTypes;
TString TCatTpcTrackUpdateHelper::GetUpdateTypeName(UpdateType_t i) { 
   return CheckUpdateType(i) ? "" : typeName[i];
}
TCatTpcTrackUpdateHelper::UpdateType_t TCatTpcTrackUpdateHelper::GetUpdateTypeValue(Int_t i) {
   return (i < 0 || i >= kNumUpdateTypes) ? kInvalid :types[i];
}


// member functions

TCatTpcTrackUpdateHelper::TCatTpcTrackUpdateHelper()
   : fUpdateType(IntVec_t(3,0)), fNameReference("reference"), fReference(NULL), 
     fOrigin(DoubleVec_t(3,0.)), fOffset(DoubleVec_t(3,0.))
{
}

TCatTpcTrackUpdateHelper::~TCatTpcTrackUpdateHelper()
{
   fReference = NULL;
}

TCatTpcTrackUpdateHelper::TCatTpcTrackUpdateHelper(const TCatTpcTrackUpdateHelper& rhs)
{
}

TCatTpcTrackUpdateHelper& TCatTpcTrackUpdateHelper::operator=(const TCatTpcTrackUpdateHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatTpcTrackUpdateHelper::Register(TProcessor *parent) 
{
   fParent = parent;
   parent->RegisterProcessorParameter("IndependentVariable","independent variable as a primary axis (0: x, 1: y, 2: z)",fIndependentVariable,0);
   parent->RegisterProcessorParameter("Origin","Origin of each axis for type 1 (fixed origin) update",
                                      fOrigin,DoubleVec_t(3,0.));
}

void TCatTpcTrackUpdateHelper::Init(TEventCollection *col)
{
   // check input and its type
   if (!fInput || !(*fInput)) { // || (**fInput)->GetClass() != TCatTpcTrack::Class()) {
      TString msg = TString::Format("Input is not set or is not a class '%s'",TCatTpcTrack::Class_Name());
      SetStateError(msg);
      return;
   }
   
   // check update type
   std::vector<TString> error;
   std::vector<Bool_t> requireVertex(3,0);
   Bool_t isRequiredVertex = kFALSE;
   for (Int_t i = 0; i < 3; ++i) {
      if (!CheckUpdateType(GetUpdateTypeValue(fUpdateType[i]))) {
         error.push_back(TString::Format("%5s %d for axis %d\n","",i,fUpdateType[i]));
      } else if (fUpdateType[i] == kExtrapolateToVertex) {
         requireVertex[i] = kTRUE;
         isRequiredVertex = kTRUE;
      }
   }
   if (error.size() > 0) {
      TString msg = "Unknown type(s) exists\n";
      for (std::vector<TString>::iterator it = error.begin(), itend = error.end(); it < itend; ++it) {
         msg += *it;
      }
      SetStateError(msg);
      return;
   }
   
   // check origin size
   if (fOrigin.size() == 0 || fOrigin.size() > 3) {
      SetStateError(TString::Format("Size of Origin should be 3: set size = %ld",fOrigin.size()));
      return;
   }

   // check offset size
   if (fOffset.size() == 0 || fOffset.size() > 3) {
      SetStateError(TString::Format("Size of Offset should be 3: set size = %ld",fOffset.size()));
      return;
   }

   // check for type kVertex and update vertex
   if (isRequiredVertex) {
      // check input collection
      fReference = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fNameReference));
      if (!fReference) {
         SetStateError(TString::Format("Reference '%s' not exist",fNameReference.Data()));
         return;
      }
      if ((*fReference)->Class() != TClonesArray::Class() ||
          (*fReference)->GetClass() != TCatTpcTrack::Class()) {
         SetStateError(TString::Format("Reference '%s' is not %s or not contains %s",fNameReference.Data(),
                                       TClonesArray::Class_Name(), TCatTpcTrack::Class_Name()));
         return;
      }
   }
   
   
}

void TCatTpcTrackUpdateHelper::OffsetToOrigin()
{
   if (!(*fInput)) return;
   Int_t numTrack = (*fInput)->GetEntries();
   for (Int_t itrack=0; itrack < numTrack; ++itrack) {
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>((*fInput)->UncheckedAt(itrack));
      TVector3& start = track->GetStartPoint();
      TVector3& end = track->GetEndPoint();
      for (Int_t i = 0; i < 3; ++i) {
         if (fUpdateType[i] != kOffsetToOrigin) continue;
         // update track by offset
         Double_t offset = start[i] - fOrigin[i];
         end[i] -= offset;
         start[i] = fOrigin[i];
         const TCatTpcTrack::TrackHitVec_t &hits = track->GetHits();
         UInt_t numHits = hits.size();
         for (Int_t ihit = 0; ihit < numHits; ++ihit) {
            hits[ihit]->GetPos()[i] -= offset;
         }
      }
   }
}

void TCatTpcTrackUpdateHelper::ExtrapolateToOrigin()
{
   if (!(*fInput)) return;
   Int_t numTrack = (*fInput)->GetEntries();
   for (Int_t itrack=0; itrack < numTrack; ++itrack) {
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>((*fInput)->UncheckedAt(itrack));
      TVector3& start = track->GetStartPoint();
      TVector3& end = track->GetEndPoint();
      TVector3& direction = track->GetDirection();
      // check the direction and correct if nessesary
      if (TMath::Abs(end[fIndependentVariable] - fOrigin[fIndependentVariable]) <
          TMath::Abs(start[fIndependentVariable] - fOrigin[fIndependentVariable])) {
         // invert start and end
         TVector3 tmp = start;
         start = end;
         end = tmp;
         direction *= -1;
      }
      // update origin
      Double_t startOriginal = start[fIndependentVariable];
      for (Int_t i = 0; i < 3; ++i) {
         if (fUpdateType[i] == kExtrapolateToOrigin) {
            if (i == fIndependentVariable) {
               start[i] = fOrigin[i];
            } else {
               start[i] = start[i] - (startOriginal - fOrigin[fIndependentVariable]) * direction[i]/direction[fIndependentVariable];
            }
         }
      }
      track->UpdateRange();
   }
}

void TCatTpcTrackUpdateHelper::StartCloseToOrigin()
{
   if (!*fInput) return;
   Int_t numTrack = (*fInput)->GetEntries();
   for (Int_t iTrack = 0; iTrack < numTrack; ++iTrack) {
      TCatTpcTrack* track = static_cast<TCatTpcTrack*>((*fInput)->UncheckedAt(iTrack));
      if ((track->GetStartPoint() - TVector3(fOrigin[0],fOrigin[1],fOrigin[2])).Dot(track->GetDirection()) < 0) {
         // direction is opposite
         TVector3 tmp = track->GetStartPoint();
         track->GetStartPoint() = track->GetEndPoint();
         track->GetEndPoint() = tmp;
         track->GetDirection() = track->GetEndPoint() - track->GetStartPoint();
      }
   }
}
