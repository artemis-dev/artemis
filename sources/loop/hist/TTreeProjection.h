/**
 * @file   TTreeProjection.h
 * @brief  Tree projection definition
 *
 * @date   Created       : 2014-03-05 10:07:22 JST
 *         Last Modified : Jun 15, 2014 11:29:15 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_C06107D1_D7C2_4FD5_A8D5_03BA80BC5DD0
#define INCLUDE_GUARD_UUID_C06107D1_D7C2_4FD5_A8D5_03BA80BC5DD0

#include "TParameterObject.h"
#include "TAttTreeProj.h"
#include "TTreeProjGroup.h"
#include <vector>

namespace art {
   class TTreeProjection;
}

class TTree;

class art::TTreeProjection : public TParameterObject {
public:
   
   TTreeProjection();
   virtual ~TTreeProjection();

   TTreeProjection(const TTreeProjection& rhs);
   TTreeProjection& operator=(const TTreeProjection& rhs);

   // @override loading yaml node
   virtual Bool_t LoadYAMLNode(const YAML::Node& node);

   // fill all the projetion
   inline virtual void Project();

   // synchronize formula with tree
   virtual Bool_t Sync(TTree *tree);
protected:
   virtual Bool_t Sync(TTreeProjGroup *group, TTree *tree, TCut cut = "");


   TObjArray *fProjGroups;
   TObjArray *fIncludes;
   TObjArray *fAliases;
   std::vector<TAttTreeProj*> fProjections;    //! helper container for fill the projections

   TString fBaseDir;

private:

   ClassDef(TTreeProjection,1) // Tree projection definition
};

inline void art::TTreeProjection::Project()
{
   std::vector<TAttTreeProj*>::iterator it;
   std::vector<TAttTreeProj*>::iterator itbegin = fProjections.begin();
   std::vector<TAttTreeProj*>::iterator itend = fProjections.end();
   for (it = itbegin; it != itend; it++) {
      (*it)->FillProjection();
   }
}
#endif // INCLUDE_GUARD_UUID_C06107D1_D7C2_4FD5_A8D5_03BA80BC5DD0
