/**
 * @file   TTreeProjectionProcessor.h
 * @brief  processor for tree projection
 *
 * @date   Created       : 2014-03-05 22:22:59 JST
 *         Last Modified : Mar 05, 2014 22:43:17 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_80AE88B4_20BD_4151_BC5D_F149BFF1B5EF
#define INCLUDE_GUARD_UUID_80AE88B4_20BD_4151_BC5D_F149BFF1B5EF

#include "TParameterLoader.h"

namespace art {
   class TTreeProjectionProcessor;
   class TTreeProjection;
}

class TTree;

class art::TTreeProjectionProcessor : public TParameterLoader {
public:
   TTreeProjectionProcessor();
   virtual ~TTreeProjectionProcessor();

   TTreeProjectionProcessor(const TTreeProjectionProcessor& rhs);
   TTreeProjectionProcessor& operator=(const TTreeProjectionProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();



protected:
   TTree *fTree; //! ttree for projection
   TTreeProjection *fTreeProj; // tree projection definition
private:

   ClassDef(TTreeProjectionProcessor,1) // processor for tree projection
};

#endif // INCLUDE_GUARD_UUID_80AE88B4_20BD_4151_BC5D_F149BFF1B5EF
