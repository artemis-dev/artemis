/* @file TCatClusterSumProcessor.h
 * @brief sum up the hits in a vector
 *
 * @date Create        : 2019-11-07 21:40:03 JST
 *       Last Modified : 2019-11-07 22:37:37 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_D46ECA34_FD55_4579_932E_763DF7B65464
#define INCLUDE_GUARD_UUID_D46ECA34_FD55_4579_932E_763DF7B65464

#include "TProcessor.h"
#include "TCatTpcTrack.h"

namespace art {
   class TCatClusterSumProcessor;
}


class art::TCatClusterSumProcessor : public TProcessor {
public:
   TCatClusterSumProcessor();
   virtual ~TCatClusterSumProcessor();

   void Init(TEventCollection *col);
   void Process();
   

protected:
   InputData<TClonesArray,TCatTpcTrack> fInput; //! input data : TClonesArray of TCatTpcTrack
   OutputData<TClonesArray, TCatTpcTrack> fOutput; //! output data : TClonesArray of TCatTpcTrack

   Parameter<Int_t> fAxisType; // type of axis along the group of the pad [0: x, 1: z]

   TClonesArray *fHitHelper; //! 
   

   ClassDef(TCatClusterSumProcessor,1)
}; // end of TCatClusterSumProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_D46ECA34_FD55_4579_932E_763DF7B65464}
