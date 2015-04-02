/**
 * @file   arthist_linkdef.h
 * @brief  linkdef file for hist package
 *
 * @date   Created       : 2014-05-15 12:40:20 JST
 *         Last Modified : May 15, 2014 14:03:23 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_534E412D_264D_495C_ADC6_16BE0E8B8C72
#define INCLUDE_GUARD_UUID_534E412D_264D_495C_ADC6_16BE0E8B8C72
#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// tree projection
#pragma link C++ class art::TAttCut+;
#pragma link C++ class art::TTreeProjGroup+;
#pragma link C++ class art::TAttTreeProj+;
#pragma link C++ class art::TAxisTreeProj+;
#pragma link C++ class art::TH1FTreeProj+;
#pragma link C++ class art::TH2FTreeProj+;
#pragma link C++ class art::TH3FTreeProj+;
#pragma link C++ class art::TTreeProjection+;
#pragma link C++ class art::TTreeProjectionProcessor;

#endif

#endif // INCLUDE_GUARD_UUID_534E412D_264D_495C_ADC6_16BE0E8B8C72
