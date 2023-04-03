/**
 * @file   TCatTpcTrackUpdateHelper.h
 * @brief  update tpc track
 *
 * @date   Created       : 2018-02-25 16:14:08 JST
 *         Last Modified : 2018-02-26 00:44:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A053C952_5735_4E48_830E_24B143E1F637
#define INCLUDE_GUARD_UUID_A053C952_5735_4E48_830E_24B143E1F637

#include "IProcessorHelper.h"
#include "TArtTypes.h"
#include <TString.h>

namespace art {
   class TCatTpcTrackUpdateHelper;
}

class TClonesArray;

class art::TCatTpcTrackUpdateHelper : public IProcessorHelper {
public:
   typedef enum { kInvalid = -1, kNone, kExtrapolateToOrigin, kOffsetToOrigin, kExtrapolateToVertex } UpdateType_t;
   static const Int_t kNumUpdateTypes;
   static TString  GetUpdateTypeName(UpdateType_t type);
   static UpdateType_t GetUpdateTypeValue(Int_t i);

   TCatTpcTrackUpdateHelper();
   virtual ~TCatTpcTrackUpdateHelper();

   TCatTpcTrackUpdateHelper(const TCatTpcTrackUpdateHelper& rhs);
   TCatTpcTrackUpdateHelper& operator=(const TCatTpcTrackUpdateHelper& rhs);

   Int_t GetUpdateType(Int_t i) { return (i>2 || i < 0) ? kInvalid : fUpdateType[i]; }
   void SetUpdateType(Int_t i, UpdateType_t iType) { 
      if (i > 2 || i < 0) return;
      if (!CheckUpdateType(iType)) return;
      fUpdateType[i] = iType;
   }

   // utility
   static Bool_t CheckUpdateType(UpdateType_t type) {
      if (type < 0 || type >= kExtrapolateToVertex) return kFALSE;
      return kTRUE;
   }

   virtual void Register(TProcessor *proc);
   virtual void Init(TEventCollection *col);
   virtual void SetUpdateTracks(TClonesArray **input) { fInput = input; }

   virtual void SetOrigin(Double_t x, Double_t y, Double_t z) {
      fOrigin.resize(3);
      fOrigin[0] = x;
      fOrigin[1] = y;
      fOrigin[2] = z;
   }

   virtual void OffsetToOrigin();
   virtual void ExtrapolateToOrigin();
   virtual void StartCloseToOrigin();

protected:
   
   TClonesArray **fInput; //! input array
   IntVec_t fUpdateType; // type of update 0: no update, 1: fixed origin, 2: offset, 3: vertex
   TString fNameReference; // name of reference
   TClonesArray **fReference; //! reference for vertex reconstruction
   DoubleVec_t fOrigin; // fixed origin
   DoubleVec_t fOffset; // offset
   Int_t fIndependentVariable; // independent variable 
private:


   ClassDef(TCatTpcTrackUpdateHelper,1) // update tpc track
};

#endif // INCLUDE_GUARD_UUID_A053C952_5735_4E48_830E_24B143E1F637
