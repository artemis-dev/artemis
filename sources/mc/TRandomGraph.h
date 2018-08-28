/**
 * @file   TRandomGraph.h
 * @brief  generate random number from graph
 *
 * @date   Created       : 2015-08-25 14:34:51 JST
 *         Last Modified : 2018-08-27 17:48:12 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_42926D7C_519C_4886_82BF_89A40D098167
#define INCLUDE_GUARD_UUID_42926D7C_519C_4886_82BF_89A40D098167

#include "TGraph.h"
#include <TRandom.h>

namespace art {
   class TRandomGraph;
}

class TSpline;

class art::TRandomGraph : public TGraph {
public:
   TRandomGraph(TGraph *graph);
   TRandomGraph(const char* filenam, const char* format);
   virtual ~TRandomGraph();

   Double_t Generate(Double_t x1, Double_t x2);
   Double_t Integral(Double_t x1, Double_t x2, Double_t step) const;

protected:
   void Init();

   TSpline *fSpline; //!
private:
   

   ClassDef(TRandomGraph,1) // generate random number from graph
};




#endif // INCLUDE_GUARD_UUID_42926D7C_519C_4886_82BF_89A40D098167
