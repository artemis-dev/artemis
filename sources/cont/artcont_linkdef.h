/**
 * @file   catcont_linkdef.h
 * @brief
 *
 * @date   Created       : 2014-10-29 18:10:16 JST
 *         Last Modified : 2020-12-08 20:57:09 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A9C48E5D_B357_486F_AD54_FCDD8B307816
#define INCLUDE_GUARD_UUID_A9C48E5D_B357_486F_AD54_FCDD8B307816

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class art::TParameterObject+;
#pragma link C++ class art::TCatReadoutPad;
#pragma link C++ class art::TCatReadoutPadArray;
#pragma link C++ class art::TCatPulseShape-;
#pragma link C++ class art::TSimpleDataBase<double>+;
#pragma link C++ class art::TSimpleDataBase<int>+;
#pragma link C++ class art::TSimpleDataBase<long>+;
#pragma link C++ class art::TSimpleData+;
#pragma link C++ class art::TSimpleDataInt+;
#pragma link C++ class art::TSimpleDataDouble+;
#pragma link C++ class art::TSimpleDataLong+;
#pragma link C++ class art::ICharge;
#pragma link C++ class art::ITiming;
#pragma link C++ class art::ITrack;
#pragma link C++ class art::IPosition;
#pragma link C++ class art::TNArray+;
#pragma link C++ class art::TNArray::Variable+;
#pragma link C++ class art::TDetectorGeometryInfo+;
#pragma link C++ class art::TTpcStaticGasProperty+;   
#pragma link C++ class art::TParameterStrings;

#endif /* __CINT__ */

#endif // INCLUDE_GUARD_UUID_A9C48E5D_B357_486F_AD54_FCDD8B307816
