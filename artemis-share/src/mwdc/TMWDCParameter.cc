/**
 * @file   TMWDCParameter.cc
 * @brief  mwdc parameter object
 *
 * @date   Created       : 2014-03-13 09:52:54 JST
 *         Last Modified : 2018-02-04 11:59:42 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TMWDCParameter.h"

#include <fstream>
#include <TROOT.h>
#include <TList.h>
#include <yaml-cpp/yaml.h>
#include "TMWDCPlaneInfo.h"

using art::TMWDCParameter;
using art::TMWDCPlaneInfo;

ClassImp(TMWDCParameter)

TMWDCParameter::TMWDCParameter()
: fIsTurned(0), fPlanes(NULL), fPlanesGlobal(NULL)
{
}

TMWDCParameter::~TMWDCParameter()
{
   delete fPlanes;
   delete fPlanesGlobal;
}

TMWDCPlaneInfo *TMWDCParameter::GetPlane(const TString& name, Bool_t isLocal)
{
   return const_cast<TMWDCPlaneInfo*>(
      static_cast<const TMWDCParameter*>(this)->GetPlane(name,isLocal));
}

const TMWDCPlaneInfo *TMWDCParameter::GetPlane(const TString& name, Bool_t isLocal) const
{
   return fLoaded ?
      (isLocal ? static_cast<TMWDCPlaneInfo*>(fPlanes->FindObject(name))
       : static_cast<TMWDCPlaneInfo*>(fPlanesGlobal->FindObject(name)))
      : NULL;
}

const TList *TMWDCParameter::GetPlaneList(Bool_t isLocal) const
{
   return isLocal ? fPlanes : fPlanesGlobal;
}

void TMWDCParameter::Print(Option_t*) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("TMWDCParameter name='%s'\n",GetName());
   if (!fLoaded) return;
   printf("%s position = (%.1lf, %.1lf, %.1lf), angle = %.2lf deg, turned = %d\n",
	  indent.Data(), fPosition[0], fPosition[1], fPosition[2],
	  fAngle, (Bool_t)fIsTurned);
   printf("%s plane_config: %s\n",indent.Data(), fConfigurationFile.Data());
}

void TMWDCParameter::Copy(TObject& obj) const
{
   TParameterObject::Copy(obj);
   TMWDCParameter &cobj = (TMWDCParameter&) obj;

   cobj.fAngle = fAngle;
   cobj.fIsTurned = fIsTurned;
   cobj.fConfigurationFile = fConfigurationFile;

   std::copy(fPosition,fPosition+kPosDim,cobj.fPosition);

   if (fPlanes) {
      if (!cobj.fPlanes) {
	 cobj.fPlanes = static_cast<TList*>(fPlanes->Clone());
      }
   } else {
      delete cobj.fPlanes;
      cobj.fPlanes = NULL;
   }

   if (fPlanesGlobal) {
      if (!cobj.fPlanesGlobal) {
	 cobj.fPlanesGlobal =
	    static_cast<TList*>(fPlanesGlobal->Clone());
      }
   } else {
      delete cobj.fPlanesGlobal;
      cobj.fPlanesGlobal = NULL;
   }
}

Bool_t TMWDCParameter::LoadYAMLNode(const YAML::Node& node)
{
   const TString kMethodName = "LoadYAMLNode";
   

   fAngle = node["angle"].as<Double_t>();
   if (node["turned"]) {
      fIsTurned = node["turned"].as<Int_t>();
   }

   { // load position
      const YAML::Node& pos = node["position"];
      const YAML::NodeType::value posType = pos.Type();
      if (posType != YAML::NodeType::Sequence
	  || pos.size() != kPosDim) {
	 Error(kMethodName,"'position' node should be a sequence with %d scalers",kPosDim);
	 return kFALSE;
      }

      for (UInt_t i = 0; i!=kPosDim; ++i ) {
         fPosition[i] = pos[i].as<Double_t>();
      }
   }

   {
      std::string filename = node["plane_config"].as<std::string>(); 
      fConfigurationFile = filename.c_str();
      std::ifstream fin(fConfigurationFile);
      if(!fin.is_open()) {
	 Error(kMethodName,"Cannot open configuration file: %s",filename.c_str());
	 return kFALSE;
      }

      if(!fPlanes) {
	 fPlanes = new TList;
	 fPlanes->SetOwner(kTRUE);
      }
      fPlanes->Clear();
      try {
	 YAML::Node config;
         config = YAML::Load(fin);
//	 YAML::Parser parser(fin);
//	 parser.GetNextDocument(config);
	 for(UInt_t i = 0; i != config.size(); ++i){
	    TMWDCPlaneInfo *info = new TMWDCPlaneInfo;
	    if (info->LoadYAMLNode(config[i].begin()->second)){
	       std::string name = config[i].begin()->first.as<std::string>();
	       info->SetName(name.c_str());
	       fPlanes->Add(info);
	    } else {
	       delete info;
	    }
	 }
      } catch (YAML::Exception& e) {
	 fprintf(stderr,
		 "Error Occurred while loading configuration file\n%s\n",
		 e.what());
	 return kFALSE;
      }
   }

   GenerateGlobal();

   return kTRUE;
}

void TMWDCParameter::OutputDescriptionYAML(std::ostream& os) const
{
   YAML::Emitter out;
   out << YAML::BeginMap;
   {
      out << YAML::Key << "position";
      out << YAML::Value << YAML::Flow << YAML::BeginSeq
	  << fPosition[0] << fPosition[1] << fPosition[2] << YAML::EndSeq;
      out << YAML::Comment("global position of MWDC");

      out << YAML::Key << "angle"
	  << YAML::Value << fAngle;
      out << YAML::Comment("rotation angle around z-axis");

      out << YAML::Key << "turned"
	  << YAML::Value << fIsTurned;
      out << YAML::Comment("put 1 if local z-axis and global z-axis is opposite");

      out << YAML::Key << "plane_config"
	  << YAML::Value
	  << (fConfigurationFile.IsNull() ? "path/to/file" : fConfigurationFile.Data());
      out << YAML::Comment("file describes plane configuration");
   }
   out << YAML::EndMap;
   os << out.c_str() << std::endl;
}

void TMWDCParameter::PrintDescriptionYAML() const
{
   OutputDescriptionYAML(std::cout);
}

void TMWDCParameter::GenerateGlobal() {
   if (!fPlanesGlobal) {
      fPlanesGlobal = new TList;
      fPlanesGlobal->SetOwner(kTRUE);
   }
   fPlanesGlobal->Clear();

   TIter next(fPlanes);
   while(const TMWDCPlaneInfo *const plane = reinterpret_cast<TMWDCPlaneInfo*>(next())) {
      TMWDCPlaneInfo *const plane_g = new TMWDCPlaneInfo(*plane);
      const Double_t z = fPosition[2] + plane->GetZ() * (fIsTurned ? -1. : 1.);
      const Double_t angle = fIsTurned ? 180. - fAngle - plane->GetAngle()
	                               : fAngle + plane->GetAngle();
      plane_g->SetX(fPosition[0]);
      plane_g->SetY(fPosition[1]);
      plane_g->SetZ(z);
      plane_g->SetAngle(angle);
      fPlanesGlobal->Add(plane_g);
   }
}

TMWDCParameter::TMWDCParameter(const TMWDCParameter& rhs)
   : TParameterObject(rhs), fAngle(rhs.fAngle), fIsTurned(rhs.fIsTurned),
     fConfigurationFile(rhs.fConfigurationFile) {
   fPlanes = rhs.fPlanes ? static_cast<TList*>(rhs.fPlanes->Clone()) : NULL;
   fPlanesGlobal = rhs.fPlanesGlobal ?
      static_cast<TList*>(rhs.fPlanesGlobal->Clone()) : NULL;
}

TMWDCParameter& TMWDCParameter::operator=(const TMWDCParameter& rhs)
{
   if (this != &rhs) {
      ((TMWDCParameter&)rhs).Copy(*this);
   }
   return *this;
}
