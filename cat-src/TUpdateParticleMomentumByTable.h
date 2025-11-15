/**
 * @file   TUpdateParticleMomentumByTable.h
 * @brief  apply energy loss to particle
 *
 * @date   Created       : 2018-08-08 10:37:59 JST
 *         Last Modified : 2018-08-09 12:15:33 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_9DC09AFF_3F51_4965_99B9_E72998C5CFA3
#define INCLUDE_GUARD_UUID_9DC09AFF_3F51_4965_99B9_E72998C5CFA3

#include "TProcessor.h"
#include <map>
#include <vector>

namespace art {
   class TUpdateParticleMomentumByTable;
}

class art::TUpdateParticleMomentumByTable : public TProcessor {
public:
   static  const Int_t kNumElementInConverterSet = 4;
   typedef  std::map<std::pair<int,int>, std::vector<Double_t> > CoeffMap_t;
   TUpdateParticleMomentumByTable();
   virtual ~TUpdateParticleMomentumByTable();

   TUpdateParticleMomentumByTable(const TUpdateParticleMomentumByTable& rhs);
   TUpdateParticleMomentumByTable& operator=(const TUpdateParticleMomentumByTable& rhs);


   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputName; // name of input file
   StringVec_t fConverterSet; // set of converter : [zint, aint, filename, order] file should contain the pair of incident energy and exit energy

   TClonesArray **fInput; //! input
   
   std::map<std::pair<int,int>, std::vector<Double_t> > fCoeff;

private:

   ClassDef(TUpdateParticleMomentumByTable,1) // apply energy loss to particle
};

#endif // INCLUDE_GUARD_UUID_9DC09AFF_3F51_4965_99B9_E72998C5CFA3
