/**
 * @file   linkdef_user.h
 * @brief
 *
 * @date   Created:       2013-08-12 14:48:07
 *         Last Modified: 2023-10-17 11:03:33 JST
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

// segment and category
#pragma link C++ class TCatFadcRawData+;
#pragma link C++ class art::TCatAtMappingProcessor;
#pragma link C++ class art::TCatReadoutPadConditionProcessor;
#pragma link C++ class art::TCatAtPedestalCalculator;
#pragma link C++ class art::TCatAtPedestalProcessor+;
#pragma link C++ class art::TCatSiCommonNoiseSubtractionProcessor+;
#pragma link C++ class art::TCatSiPedestalSubtractionProcessor+;
#pragma link C++ class art::TCatSiHitFindProcessor+;
#pragma link C++ class art::TCatAtPedestalSubtractionProcessor+;
#pragma link C++ class art::TCatAtHitPatternProcessor+;
#pragma link C++ class art::TCatAtSiHitPatternProcessor+;
//#pragma link C++ class art::TCatSiHitPatternProcessor+;
#pragma link C++ class art::TCatSumSampleProcessor;
#pragma link C++ class art::TCatAtSpillCounter+;
#pragma link C++ class art::TCatDiffSampleProcessor;
#pragma link C++ class art::TCatScalerData+;
#pragma link C++ class art::TCatScalerMappingProcessor+;
//#pragma link C++ class art::TCatPulseShape+;
#pragma link C++ class art::TCatGausSampleProcessor+;
#pragma link C++ class art::TTimingChargeData+;
#pragma link C++ class art::TTimingChargeMappingProcessor;
#pragma link C++ class art::TTimingChargePositionData+;
#pragma link C++ class art::TTimingChargePositionMappingProcessor;
#pragma link C++ class art::TTwoSidedPlasticData+;
#pragma link C++ class art::TTwoSidedPlasticCalibrationProcessor;
#pragma link C++ class art::TConverterBase+;
#pragma link C++ class art::TAffineConverter+;
#pragma link C++ class art::TAffineConverterArrayGenerator;
#pragma link C++ class art::TPolynomialConverter+;
#pragma link C++ class art::TPolynomialConverterGenerator;
#pragma link C++ class art::TPolynomialConverterArrayGenerator;
#pragma link C++ class art::TAffineConverterCombinator;

#pragma link C++ class art::T1DGatingConverter+;
#pragma link C++ class art::TMWDCPlaneInfo+;
#pragma link C++ class art::TMWDCHitData+;
#pragma link C++ class art::TTrack+;
#pragma link C++ class art::TMWDCTrackingResult+;
#pragma link C++ class art::TMonotoneTableConverter+;
#pragma link C++ class art::TMonotoneTableConverterGenerator;
#pragma link C++ class art::TMWDCPlaneProcessor;
#pragma link C++ class art::TMWDCCalibrationProcessor;
#pragma link C++ class art::TMWDCTrackingProcessor;
#pragma link C++ class art::TMWDCPlaneCalibrationTask;
#pragma link C++ class art::TGMWDCCalibration;
#pragma link C++ class art::TGMWDCConfig;
#pragma link C++ class art::TGMWDCConfigGroupFrame;
#pragma link C++ class art::TTimeReferenceProcessor;
#pragma link C++ class art::TPPACData+;
#pragma link C++ class art::TPPACProcessor;
#pragma link C++ class art::TPPACParameter+;
#pragma link C++ class art::TPPACTrackingProcessor;
#pragma link C++ class art::TCoinRegMappingProcessor;
#pragma link C++ class art::TCatPulseBitCorrectionProcessor;
#pragma link C++ class art::TPulseUnfoldHighPassProcessor;
#pragma link C++ class art::TPulseUnfoldLowPassProcessor;
#pragma link C++ class art::TPulseFinder;
#pragma link C++ class art::THitFinder;
#pragma link C++ class art::TPulseMovingAverageProcessor;
#pragma link C++ class art::TSampleCalibratorProcessor;
#pragma link C++ class art::TCatRecoilFinder;
#pragma link C++ class art::TTimingChargeCalibrationProcessor;
#pragma link C++ class art::TTimingSubtractionProcessor;
#pragma link C++ class art::TTimeDifference+;
#pragma link C++ class art::TMultiHitArray+;
#pragma link C++ class art::TTimingData+;
#pragma link C++ class art::TChargeData+;
#pragma link C++ class art::TRIDFEventShifter;
#pragma link C++ class art::TDataCalibrationProcessor;
#pragma link C++ class art::TMWDCParameter;
#pragma link C++ class art::TMultiHitCombinator;
#pragma link C++ class art::sh04::TRecoilProcessor;
#pragma link C++ class art::sh04::TRecoilTOFProcessor;
#pragma link C++ class art::sh04::TP2PKinematicsData;
#pragma link C++ class art::sh04::TP2PKinematicsProcessor;
#pragma link C++ class art::sh04::TSMWDCTrackingProcessor;
#pragma link C++ class art::sh04::TSMWDCDriftLengthCalibrator;
//#pragma link C++ class art::TCatReadoutPad;
//#pragma link C++ class art::TCatReadoutPadArray;
#pragma link C++ class art::TCoinRegGateProcessor;
#pragma link C++ class art::TCoinRegData;
#pragma link C++ class art::TGateArray;
#pragma link C++ class art::TGateObject;
#pragma link C++ class art::TGateInfo;
#pragma link C++ class art::TGateArrayInitializer;
#pragma link C++ class art::TPIDGateProcessor;
#pragma link C++ class art::TTreeFormulaGateProcessor;
#pragma link C++ class art::TGenericData;
#pragma link C++ class art::TTrackXYGateProcessor;
#pragma link C++ class art::TGateStopProcessor;
#pragma link C++ class art::sh04::TDetectorGeometry;
#pragma link C++ class art::sh04::TBeamParameter;
#pragma link C++ class art::sh04::TBeamZ;
#pragma link C++ class art::sh04::TBeamZProcessor;
#pragma link C++ class art::sh04::TReactionVertexProcessor;
#pragma link C++ class art::sh04::TTrackProjectionProcessor;
#pragma link C++ class art::TSeparateOutputProcessor;
#pragma link C++ class art::TExtractOutputProcessor;
#pragma link C++ class art::TWalkConverter+;
#pragma link C++ class art::TWalkCorrectionProcessor;
#pragma link C++ class art::TFixedNumberParameter;
#pragma link C++ class art::TCmdMWDCCalib;
#pragma link C++ class art::TCmdMWDCConfig;
#pragma link C++ class art::TCmdFiga;
#pragma link C++ class art::sh04::TNyokiGateProcessor;
#pragma link C++ class art::sh04::TNyokiTimingProcessor;
#pragma link C++ class art::sh04::TSlewCorrectionProcessor;
#pragma link C++ class art::sh04::TParticleIdentifier;
#pragma link C++ class art::sh04::TResidualPIDProcessor;
#pragma link C++ class art::sh04::TS1PIDProcessor;
#pragma link C++ class art::sh04::TNyokiChargeCorrectionProcessor;
#pragma link C++ class art::TSplineConverter+;
#pragma link C++ class art::TMultiFileParameterArrayLoader;
#pragma link C++ class art::TMWDCDriftLengthCorrectionProcessor;
#pragma link C++ class art::TMWDCPlaneDLCorrectionProcessor;
#pragma link C++ class art::TMergeTrackProcessor;

//RCNP
#pragma link C++ class art::TRCNPEventHeader;
#pragma link C++ class art::TRCNPEventStore_ts;
#pragma link C++ class art::TRCNPRunInfo;
#pragma link C++ class art::TVDCCluster;
#pragma link C++ class art::TVDCClusterizationProcessor;
#pragma link C++ class art::TVDCPlaneProcessor;
#pragma link C++ class art::TVDCTrackingProcessor;
#pragma link C++ class art::TVDCTrackingProcessor_gr;
#pragma link C++ class art::TVDCTrackingProcessor_las;
#pragma link C++ class art::TVDCTrackingResult;
//RCNP decoder
#pragma link C++ class art::TModuleDecoderRCNP;
#pragma link C++ class art::TModuleDecoder3377;
#pragma link C++ class art::TModuleDecoderFERA;
#pragma link C++ class art::TModuleDecoderFERET;
#pragma link C++ class art::TModuleDecoderTS;
#pragma link C++ class art::TModuleDecoderV830;
#pragma link C++ class art::TModuleDecoderV1190_rcnp;
#pragma link C++ class art::TModuleDecoderMXDC32_rcnp;

#endif // __CINT__

#endif // LINKDEF_USER_H

