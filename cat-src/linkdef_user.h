/**
 * @file   linkdef_user.h
 * @brief
 *
 * @date   Created:       2013-08-12 14:48:07
 *         Last Modified: 2019-11-07 20:00:41 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro, OTA Shinsuke All rights reserved
 */
#ifndef LINKDEF_USER_H
#define LINKDEF_USER_H


#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class art::TModuleDecoderA3100;
#pragma link C++ class art::TModuleDecoderA3100TriggeredList;
#pragma link C++ class art::TModuleDecoderHINP;
#pragma link C++ class art::TSimpleDataMappingProcessor;
#pragma link C++ class art::TChargeValidateProcessor;
#pragma link C++ class art::TModuleDecoderSIS3301;
#pragma link C++ class art::TMonitorMappingProcessor;
#pragma link C++ class art::TCatBeamTrackSimulator;
#pragma link C++ class art::TCatAtSaveHitPatternProcessor+;
#pragma link C++ class art::TCatAtEventSelectionByRecoilHitPattern;
#pragma link C++ class art::TCatAtRoughEventCounter;
#pragma link C++ class art::TCatAtPedestalSubtractTester;
#pragma link C++ class art::TPulseUnfoldHighPassTester;
#pragma link C++ class art::TSampleCalibratorTester;
#pragma link C++ class art::TPulseFindTester;
#pragma link C++ class art::TGraphDrawer;
#pragma link C++ class art::TPulseDrawer;
#pragma link C++ class art::TCatGainEvaluationProcessor;
#pragma link C++ class TCmdXsta;
#pragma link C++ class art::TCatHitFinderByCharge;
#pragma link C++ class art::THogeProcessor;
#pragma link C++ class art::TModuleDecoderV1740_mod;
#pragma link C++ class art::TCatAtSaveChargePatternProcessor+;
#pragma link C++ class art::TCatBeamXProcessor+;
#pragma link C++ class art::TCatRefineRecoilClusterProcessor;
#pragma link C++ class art::TEventSkipProcessor;
#pragma link C++ class art::TCatTrackResultPulse+;
#pragma link C++ class art::TCatAtClusteringProcessor;
#pragma link C++ class art::TCoinRegUnGateProcessor;
#pragma link C++ class art::TBetaDecomposeProcessor;
#pragma link C++ class art::TBrhoReconstructProcessor;
#pragma link C++ class art::ms1502::TPIDProcessor;
#pragma link C++ class art::TCatTPCRecoilCandidateSelectProcessor;
#pragma link C++ class art::TCatTPCRecoilCandidateRescanByDistanceProcessor;
//#pragma link C++ class art::TCatCentroidTrackingProcessor;
#pragma link C++ class art::TEventDisplayProcessorHelper;
#pragma link C++ class art::TRangeTableHelper;
#pragma link C++ class art::TCatTrackMinuitHelper;
#pragma link C++ class art::TCatTrackDiffusedMinuitHelper;
#pragma link C++ class art::TCatTrackingWithDiffusedChargeProcessor;
#pragma link C++ class art::TCatTrackResult;
#pragma link C++ class art::TCatTrackResultDisplayProcessor;
#pragma link C++ class art::TCatMissingMassProcessor;
#pragma link C++ class art::TCatAtBeamInRecoilRegionCounter;
#pragma link C++ class art::TCatAtSaveMaxSamplePatternProcessor;
#pragma link C++ class art::TCatRegionHitsDivider;
#pragma link C++ class art::TPPACAnodeTimingProcessor;
#pragma link C++ class art::TPPACProcessorQTC;
#pragma link C++ class art::TRawDataTriggeredList;
#pragma link C++ class art::TCatDiffusedChargeResponseHelper;
#pragma link C++ class art::TRangeTableConditionProcessor;
#pragma link C++ class art::TCatRecoilEventGenerator;
#pragma link C++ class art::TAffineConverterConditionProcessor;
#pragma link C++ class art::TCatTpcHitClusterProcessor;
#pragma link C++ class art::TCatTpcTrack-;
#pragma link C++ class art::TCatBeamHitsFinderWithRecoil;
#pragma link C++ class art::TParticleGenerateProcessor;
#pragma link C++ class art::TUpdateParticleMomentumProcessor;
#pragma link C++ class art::TMissingMassProcessor;
#pragma link C++ class art::TTimingChargeTriggeredListMappingProcessor;
#pragma link C++ class art::TArtemisCanvas;
#pragma link C++ class art::TPulsePoleZeroCancellationProcessor;
#pragma link C++ class art::TCatTpcTrackUpdateHelper;
#pragma link C++ class art::TNArrayLoader;
#pragma link C++ class art::TTrackGenerator;
#pragma link C++ class art::TEventInfoGenerator;
#pragma link C++ class art::TTpcEventGenerator;
#pragma link C++ class art::TPIDwSinglePlaneProcessor;
#pragma link C++ class art::TPIDwSinglePPACPlaneProcessor;
#pragma link C++ class art::TGenerateParticleFromPID;
#pragma link C++ class art::TUpdateParticleMomentumByBrho;
#pragma link C++ class art::TUpdateParticleMomentumByTable;
#pragma link C++ class art::TCatTrackFittingWithSiliconProcessor;
#pragma link C++ class art::TSiliconEventGenerator;
#pragma link C++ class art::ribf113::TCatGlastPositionConditionProcessor; 
#pragma link C++ class art::ribf113::TCatTrackFindWithSiliconProcessor;
#pragma link C++ class art::TEnergyCorrectionInMaterial; 
#pragma link C++ class art::TDetectorGeometryUpdateProcessor;   
#pragma link C++ class art::TChargeRiseTimeCompensateProcessor;
#pragma link C++ class art::TTimestampCalibrationProcessor;
#pragma link C++ class art::TCatClusterSumProcessor;

#pragma link C++ class art::TTpcStaticGasPropertyConditionProcessor;
#pragma link C++ class art::TResponseFunctionNArray; 
// sr-ppac related
#pragma link C++ class art::TSRPPACPlaneData+;
#pragma link C++ class art::TSRPPACPlaneProcessor;
#pragma link C++ class art::TSRPPACPlaneDqdxProcessor;
#pragma link C++ class art::TSRPPACParameter;
#pragma link C++ class art::TSRPPACPlaneCalibrationTask;
#pragma link C++ class art::TGSRPPACCalibration;
#pragma link C++ class art::TSRPPACCalibrationProcessor;

// cat
#pragma link C++ class art::TCatEvalTpcChargeResolution;
#pragma link C++ class art::TCatEvalGainShiftProcessor;

#endif // __CINT__

#endif // LINKDEF_USER_H

