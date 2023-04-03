/**
 * @file   TSRPPACParameter.cc
 * @brief  Parameter class for SR-PPAC
 *
 * @date   Created       : 2019-02-04 03:20:38 DST
 *         Last Modified : 2019-02-16 08:11:14 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shoichiro Masuoka
 */

#include "TSRPPACParameter.h"
#include <yaml-cpp/yaml.h>
#include "constant.h"

using art::TSRPPACParameter;


ClassImp(TSRPPACParameter)

TSRPPACParameter::TSRPPACParameter()
// : fNStrip(kInvalidI), fStripWidth(kInvalidD), fCenter(kInvalidD),
//    fDetOffset(kInvalidD), fZ(kInvalidD), fTurned(kFALSE)
{
   RegisterParameter("nStrip", "Number of strip",fNStrip,(Int_t)0, 1);
   RegisterParameter("stripWidth", "strip width (mm)", fStripWidth, (Double_t)0., 1);
   RegisterParameter("center", "center of ID", fCenter,(Double_t)0., 1);
   RegisterParameter("detoffset", "detector offset", fDetOffset, (Double_t)0., 1);
   RegisterParameter("z", "z postion from a focul plane (mm)", fZ, (Double_t)0., 1);
   RegisterParameter("turned","1: detector is turned", fTurned, (Bool_t)kFALSE,1);
}

TSRPPACParameter::~TSRPPACParameter()
{
}

// Bool_t TSRPPACParameter::LoadYAMLNode(const YAML::Node &node)
// {
//    // if (!TParameterObject::LoadYAMLNode(node)) return kFALSE;
//    // return kTRUE;
//    try {
//       node["nStrip"] >> fNStrip;
//       node["stripWidth"] >> fStripWidth;
//       node["center"] >> fCenter;
//       node["detoffset"] >> fDetOffset;
//       node["z"] >> fZ;
//       node["turned"] >> fTurned;
//    } catch (YAML::Exception& e) {
//       Error("LoadYAMLNode", "Error occurred");
//       fprintf(stderr, "%s\n",e.what());
//       return kFALSE;
//    }
//    return kTRUE;
// }

