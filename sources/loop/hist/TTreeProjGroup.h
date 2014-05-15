/**
 * @file   TTreeProjGroup.h
 * @brief  Collection of the projections of TTree
 *
 * @date   Created       : 2014-03-03 16:59:53 JST
 *         Last Modified : Mar 05, 2014 16:47:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842
#define INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842

#include "TDirectoryFile.h"
#include "TAttCut.h"

namespace art {
   class TTreeProjGroup;
}

class art::TTreeProjGroup : public TDirectoryFile, public TAttCut {
public:
   TTreeProjGroup();
   TTreeProjGroup(const char *name, const char *title, const char *cut = "", TDirectory *mother = 0);
   virtual ~TTreeProjGroup();

   TTreeProjGroup(const TTreeProjGroup& rhs);
   TTreeProjGroup& operator=(const TTreeProjGroup& rhs);

   virtual TObject* Clone(const char *newname = "") const;

protected:

private:

   ClassDef(TTreeProjGroup,1) // Collection of the projections of TTree
};

#endif // INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842
