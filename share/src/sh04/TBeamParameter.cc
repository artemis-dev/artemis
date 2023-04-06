/**
 * @file   TBeamParameter.cc
 * @brief
 *
 * @date   Created:       2014-02-24 10:27:20
 *         Last Modified: 2014-06-17 14:08:25 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TBeamParameter.h"

#include <yaml-cpp/yaml.h>

#include <TClass.h>

using art::sh04::TBeamParameter;

ClassImp(art::sh04::TBeamParameter)

// Default constructor
TBeamParameter::TBeamParameter()
   : fZ(1), fA(1), fBrho(0), fF6XD(0) {
   RegisterParameter("Z","proton number",fZ,1,1);
   RegisterParameter("A","mass number",fA,1,1);
   RegisterParameter("Brho","magnetic rigidity",fBrho,0.F,1);
   RegisterOptionalParameter("F6XDelta","momentum dispersion at F6 focal plane",fF6XD,0.F,1);
}

TBeamParameter::~TBeamParameter() {
}

// // Copy constructor
// TBeamParameter::TBeamParameter(const TBeamParameter& rhs) {
// }

// // Assignment operator
// TBeamParameter& TBeamParameter::operator=(const TBeamParameter& rhs) {
//    if (this != &rhs) {
//       // TODO: implement copy
//    }
//    return *this;
//}

void TBeamParameter::Print(Option_t*) const {
   printf("OBJ: %s\t%s\tZ = %d,\tA = %d,\tBrho = %.3lf,\tF6XD = %lf\n",
	  IsA()->GetName(), GetName(), fZ, fA, fBrho, fF6XD);
}

void TBeamParameter::PrintDescriptionYAML() const {
   YAML::Emitter out;
   out << YAML::BeginMap;
   {
      out << YAML::Key << "Z" << YAML::Value << fZ;
      out << YAML::Comment("proton number");

      out << YAML::Key << "A" << YAML::Value << fA;
      out << YAML::Comment("mass number");

      out << YAML::Key << "Brho" << YAML::Value << fBrho;
      out << YAML::Comment("magnetic rigidity");

      out << YAML::Key << "F6XDelta" << YAML::Value << fF6XD;
      out << YAML::Comment("momentum dispersion at F6 focal plane");
   }
   out << YAML::EndMap;
   std::cout << out.c_str() << std::endl;

}
