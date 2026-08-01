# Install script for directory: /home/ota/repos/artemis/artemis-share/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/ota/.local/artemis/v2-0-0-v6-40-02")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/opt/rh/gcc-toolset-15/root/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ota/repos/artemis/artemis-share/src/libartshare.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so"
         OLD_RPATH "/home/ota/.local/root/v6-40-02/lib:/home/ota/build/yaml-cpp/0.6.3/../..:/home/ota/repos/artemis/sources/mc:/usr/lib64/openmpi/lib:/home/ota/repos/artemis/sources/commands:/home/ota/repos/artemis/sources/loop/streaming-v1:/home/ota/repos/artemis/sources/loop/streaming:/home/ota/repos/artemis/sources/loop/ridf:/home/ota/repos/artemis/sources/loop/rdf:/home/ota/repos/artemis/sources/loop/hist:/home/ota/repos/artemis/sources/loop/param:/home/ota/repos/artemis/sources/loop/decoder:/home/ota/repos/artemis/sources/loop:/home/ota/repos/artemis/sources/cont:/home/ota/repos/artemis/sources/core:/home/ota/build/yaml-cpp/0.6.3:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/opt/rh/gcc-toolset-15/root/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libartshare.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/ota/repos/artemis/artemis-share/src/TWalkCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TTimingSubtractionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TTimingChargePositionMappingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TTimingChargeMappingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TTimingChargeCalibrationProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TTimeReferenceProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TSeparateOutputProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TRIDFEventShifter.h"
    "/home/ota/repos/artemis/artemis-share/src/TMultiHitCombinator.h"
    "/home/ota/repos/artemis/artemis-share/src/TMultiHitArray.h"
    "/home/ota/repos/artemis/artemis-share/src/TMultiFileParameterArrayLoader.h"
    "/home/ota/repos/artemis/artemis-share/src/TExtractOutputProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TDataCalibrationProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/TMergeTrackProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/constant.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TSampleCalibratorProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TPulseUnfoldLowPassProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TPulseUnfoldHighPassProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TPulseMovingAverageProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TPulseFinder.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/THitFinder.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatSumSampleProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatSiPedestalSubtractionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatSimplestData.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatSiHitFindProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatSiCommonNoiseSubtractionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatRecoilFinder.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatReadoutPadConditionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatPulseBitCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatHitSelection.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatGausSampleProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatFadcRawData.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatDiffSampleProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtSpillCounter.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtSiHitPatternProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtPedestalSubtractionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtPedestalProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtPedestalCalculator.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtMappingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/cat/TCatAtHitPatternProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/coin/TCoinRegMappingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/coin/TCoinRegGateProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/command/TCmdMWDCConfig.h"
    "/home/ota/repos/artemis/artemis-share/src/command/TCmdMWDCCalib.h"
    "/home/ota/repos/artemis/artemis-share/src/command/TCmdFiga.h"
    "/home/ota/repos/artemis/artemis-share/src/command/TCatCmdListg.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTrack.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTrackerHit3D.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTimingData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTimingChargePositionData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTimingChargeData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TTimeDifference.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TGenericData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TCoinRegData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TChargeData.h"
    "/home/ota/repos/artemis/artemis-share/src/cont/TCatScalerData.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TWalkConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TSplineConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TPolynomialConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TPolynomialConverterGenerator.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TPolynomialConverterArrayGenerator.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TMonotoneTableConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TMonotoneTableConverterGenerator.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TFixedNumberParameter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TConverterUtil.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TConverterBase.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TAffineConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TAffineConverterCombinator.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/TAffineConverterArrayGenerator.h"
    "/home/ota/repos/artemis/artemis-share/src/converter/T1DGatingConverter.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TTreeFormulaGateProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TTrackXYGateProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TPIDGateProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TGateStopProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TGateObject.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TGateInfo.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TGateArrayInitializer.h"
    "/home/ota/repos/artemis/artemis-share/src/gate/TGateArray.h"
    "/home/ota/repos/artemis/artemis-share/src/plastic/TTwoSidedPlasticData.h"
    "/home/ota/repos/artemis/artemis-share/src/plastic/TTwoSidedPlasticCalibrationProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCTrackingResult.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCTrackingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCPlaneProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCPlaneInfo.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCPlaneDLCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCPlaneCalibrationTask.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCParameter.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCHitData.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCDriftLengthCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TMWDCCalibrationProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TGMWDCConfig.h"
    "/home/ota/repos/artemis/artemis-share/src/mwdc/TGMWDCCalibration.h"
    "/home/ota/repos/artemis/artemis-share/src/ppac/TPPACTrackingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/ppac/TPPACProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/ppac/TPPACParameter.h"
    "/home/ota/repos/artemis/artemis-share/src/ppac/TPPACData.h"
    "/home/ota/repos/artemis/artemis-share/src/scaler/TCatScalerMappingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCTrackingResult.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCTrackingProcessor_las.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCTrackingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCTrackingProcessor_gr.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCPlaneProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCClusterizationProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TVDCCluster.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TRCNPRunInfo.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TRCNPEventStore_ts.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp/TRCNPEventHeader.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderV830.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderV1190_rcnp.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderTS.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderRCNP.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderMXDC32_rcnp.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderFERET.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoderFERA.h"
    "/home/ota/repos/artemis/artemis-share/src/rcnp_decoder/TModuleDecoder3377.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TTrackProjectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TSMWDCTrackingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TSMWDCDriftLengthCalibrator.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TSlewCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TS1PIDProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TResidualPIDProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TRecoilTOFProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TRecoilProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TReactionVertexProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TParticleIdentifier.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TP2PKinematicsProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TP2PKinematicsData.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TNyokiTimingProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TNyokiGateProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TNyokiChargeCorrectionProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TDetectorGeometry.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TBeamZProcessor.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TBeamZ.h"
    "/home/ota/repos/artemis/artemis-share/src/sh04/TBeamParameter.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/home/ota/repos/artemis/artemis-share/src/libartshare_rdict.pcm")
endif()

