/**
 * @file   TTreeProjComponent.cc
 * @brief  Collection of the projections of TTree
 *
 * @date   Created       : 2023-06-28 00:19:01 JST
 *         Last Modified : 2024-06-11 19:32:28 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TTreeProjComponent.h"

ClassImp(art::TTreeProjComponent)


art::TTreeProjComponent::TTreeProjComponent()
{

}

art::TTreeProjComponent::TTreeProjComponent(const char* name, const char* title)
: TNamed(name,title)
{
}

art::TTreeProjComponent::~TTreeProjComponent()
{
}

void art::TTreeProjComponent::Copy(TObject& obj) const 
{
    TNamed::Copy(obj);
}
