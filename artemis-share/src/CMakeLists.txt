set(ARTEMIS_INCLUDE_DIRS
    ${ARTEMIS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/artemis-share/src
    ${CMAKE_SOURCE_DIR}/artemis-share/src/cat
    ${CMAKE_SOURCE_DIR}/artemis-share/src/coin
    ${CMAKE_SOURCE_DIR}/artemis-share/src/command
    ${CMAKE_SOURCE_DIR}/artemis-share/src/cont
    ${CMAKE_SOURCE_DIR}/artemis-share/src/converter
    ${CMAKE_SOURCE_DIR}/artemis-share/src/gate
    ${CMAKE_SOURCE_DIR}/artemis-share/src/mwdc
    ${CMAKE_SOURCE_DIR}/artemis-share/src/plastic
    ${CMAKE_SOURCE_DIR}/artemis-share/src/ppac
    ${CMAKE_SOURCE_DIR}/artemis-share/src/rcnp
    ${CMAKE_SOURCE_DIR}/artemis-share/src/rcnp_decoder
    ${CMAKE_SOURCE_DIR}/artemis-share/src/scaler
    ${CMAKE_SOURCE_DIR}/artemis-share/src/sh04
    CACHE INTERNAL ""
)

set(OBJECTS
# cat
    cat/TSampleCalibratorProcessor.cc
    cat/TCatAtMappingProcessor.cc
    cat/TCatReadoutPadConditionProcessor.cc
    cat/TCatAtPedestalCalculator.cc
    cat/TCatAtPedestalProcessor.cc
    cat/TCatAtHitPatternProcessor.cc
    cat/TCatAtSiHitPatternProcessor.cc
    cat/TCatPulseBitCorrectionProcessor.cc
    cat/TCatAtSpillCounter.cc
    cat/TCatAtPedestalSubtractionProcessor.cc
    cat/TCatSiCommonNoiseSubtractionProcessor.cc
    cat/TCatSiPedestalSubtractionProcessor.cc
    cat/TCatSiHitFindProcessor.cc
    cat/TPulseUnfoldHighPassProcessor.cc
    cat/TPulseUnfoldLowPassProcessor.cc
    cat/TPulseFinder.cc
    cat/THitFinder.cc
    cat/TPulseMovingAverageProcessor.cc
    cat/TCatSumSampleProcessor.cc
    cat/TCatDiffSampleProcessor.cc
    cat/TCatGausSampleProcessor.cc
    cat/TCatFadcRawData.cc
    cat/TCatRecoilFinder.cc
# cont
    cont/TGenericData.cc
    cont/TTimingData.cc
    cont/TChargeData.cc
    cont/TTimingChargeData.cc
    cont/TTimingChargePositionData.cc
    cont/TCatScalerData.cc
    cont/TTrack.cc
    cont/TTimeDifference.cc
    cont/TCoinRegData.cc
# decoder
# scaler
    scaler/TCatScalerMappingProcessor.cc
# converter
    converter/TConverterUtil.cc
    converter/TConverterBase.cc
    converter/TAffineConverter.cc
    converter/TAffineConverterArrayGenerator.cc
    converter/TPolynomialConverter.cc
    converter/TPolynomialConverterGenerator.cc
    converter/TPolynomialConverterArrayGenerator.cc
    converter/TMonotoneTableConverter.cc
    converter/TMonotoneTableConverterGenerator.cc
    converter/T1DGatingConverter.cc
    converter/TSplineConverter.cc
    converter/TAffineConverterCombinator.cc
    converter/TWalkConverter.cc
    converter/TFixedNumberParameter.cc
# gate
    gate/TGateArray.cc
    gate/TGateObject.cc
    gate/TGateInfo.cc
    gate/TGateArrayInitializer.cc
    gate/TPIDGateProcessor.cc
    gate/TTrackXYGateProcessor.cc
    gate/TGateStopProcessor.cc
    sh04/TNyokiGateProcessor.cc
    gate/TTreeFormulaGateProcessor.cc
# plastic
    plastic/TTwoSidedPlasticData.cc
    plastic/TTwoSidedPlasticCalibrationProcessor.cc
# ppac
    ppac/TPPACData.cc
    ppac/TPPACProcessor.cc
    ppac/TPPACParameter.cc
    ppac/TPPACTrackingProcessor.cc
# mwdc
    mwdc/TMWDCPlaneInfo.cc
    mwdc/TMWDCHitData.cc
    mwdc/TMWDCTrackingResult.cc
    mwdc/TMWDCPlaneProcessor.cc
    mwdc/TMWDCCalibrationProcessor.cc
    mwdc/TMWDCTrackingProcessor.cc
    mwdc/TMWDCParameter.cc
    mwdc/TMWDCPlaneCalibrationTask.cc
    mwdc/TGMWDCCalibration.cc
    mwdc/TGMWDCConfig.cc
    mwdc/TMWDCDriftLengthCorrectionProcessor.cc
    mwdc/TMWDCPlaneDLCorrectionProcessor.cc
# coin
    coin/TCoinRegMappingProcessor.cc
    coin/TCoinRegGateProcessor.cc
# others
    TSeparateOutputProcessor.cc
    TExtractOutputProcessor.cc
    TWalkCorrectionProcessor.cc
    TMultiFileParameterArrayLoader.cc
    TTimingChargeMappingProcessor.cc
    TTimingChargePositionMappingProcessor.cc
    TTimeReferenceProcessor.cc
    TTimingChargeCalibrationProcessor.cc
    TTimingSubtractionProcessor.cc
    TMultiHitArray.cc
    TMultiHitCombinator.cc
    TRIDFEventShifter.cc
    TDataCalibrationProcessor.cc
    TMergeTrackProcessor.cc
# sh04
    sh04/TDetectorGeometry.cc
    sh04/TRecoilProcessor.cc
    sh04/TRecoilTOFProcessor.cc
    sh04/TP2PKinematicsData.cc
    sh04/TP2PKinematicsProcessor.cc
    sh04/TSMWDCTrackingProcessor.cc
    sh04/TSMWDCDriftLengthCalibrator.cc
    sh04/TBeamParameter.cc
    sh04/TBeamZ.cc
    sh04/TBeamZProcessor.cc
    sh04/TReactionVertexProcessor.cc
    sh04/TSlewCorrectionProcessor.cc
    sh04/TNyokiTimingProcessor.cc
    sh04/TNyokiChargeCorrectionProcessor.cc
    sh04/TTrackProjectionProcessor.cc
    sh04/TParticleIdentifier.cc
    sh04/TResidualPIDProcessor.cc
    sh04/TS1PIDProcessor.cc
# command
    command/TCmdMWDCCalib.cc
    command/TCmdMWDCConfig.cc
    command/TCmdFiga.cc
# rcnp
    rcnp/TRCNPEventHeader.cc
    rcnp/TRCNPEventStore_ts.cc
    rcnp/TRCNPRunInfo.cc
    rcnp/TVDCCluster.cc
    rcnp/TVDCClusterizationProcessor.cc
    rcnp/TVDCPlaneProcessor.cc
    rcnp/TVDCTrackingProcessor.cc
    rcnp/TVDCTrackingProcessor_gr.cc
    rcnp/TVDCTrackingProcessor_las.cc
    rcnp/TVDCTrackingResult.cc
# rcnp_decoder
    rcnp_decoder/TModuleDecoderRCNP.cc
    rcnp_decoder/TModuleDecoder3377.cc
    rcnp_decoder/TModuleDecoderFERA.cc
    rcnp_decoder/TModuleDecoderFERET.cc
    rcnp_decoder/TModuleDecoderTS.cc
    rcnp_decoder/TModuleDecoderV830.cc
    rcnp_decoder/TModuleDecoderV1190_rcnp.cc
    rcnp_decoder/TModuleDecoderMXDC32_rcnp.cc

)

set(CAT_HEADERS
    TSampleCalibratorProcessor.h
    TPulseUnfoldLowPassProcessor.h
    TPulseUnfoldHighPassProcessor.h
    TPulseMovingAverageProcessor.h
    TPulseFinder.h
    THitFinder.h
    #cat/TCatV1740AdcCorrectionProcessor.h
    TCatSumSampleProcessor.h
    TCatSiPedestalSubtractionProcessor.h
    TCatSimplestData.h
    #cat/TCatSimpleCutProcessor.h
    TCatSiHitFindProcessor.h
    TCatSiCommonNoiseSubtractionProcessor.h
    TCatRecoilFinder.h
    TCatReadoutPadConditionProcessor.h
    TCatPulseBitCorrectionProcessor.h
    TCatHitSelection.h
    TCatGausSampleProcessor.h
    TCatFadcRawData.h
    TCatDiffSampleProcessor.h
    #cat/TCatChargeCutProcessor.h
    TCatAtSpillCounter.h
    TCatAtSiHitPatternProcessor.h
    TCatAtPedestalSubtractionProcessor.h
    TCatAtPedestalProcessor.h
    TCatAtPedestalCalculator.h
    TCatAtMappingProcessor.h
    TCatAtHitPatternProcessor.h
    #cat/TCatAtHitFindProcessor.h
    #cat/TCatAtHitCandidateProcessor.h
    #cat/TCatAtChargeRatioProcessor.h
    #cat/TCatAtBaseLineSubractionProsessor.h
)
set(COIN_HEADERS
    TCoinRegMappingProcessor.h
    TCoinRegGateProcessor.h
)
set(COMMAND_HEADERS
    TCmdMWDCConfig.h
    TCmdMWDCCalib.h
    TCmdFiga.h
    TCatCmdListg.h
)
set(CONT_HEADERS
    TTrack.h
    TTrackerHit3D.h
    TTimingData.h
    TTimingChargePositionData.h
    TTimingChargeData.h
    TTimeDifference.h
    TGenericData.h
    TCoinRegData.h
    TChargeData.h
    TCatScalerData.h
)
set(CONVERTER_HEADERS
    TWalkConverter.h
    TSplineConverter.h
    TPolynomialConverter.h
    TPolynomialConverterGenerator.h
    TPolynomialConverterArrayGenerator.h
    TMonotoneTableConverter.h
    TMonotoneTableConverterGenerator.h
    TFixedNumberParameter.h
    TConverterUtil.h
    TConverterBase.h
    TAffineConverter.h
    TAffineConverterCombinator.h
    TAffineConverterArrayGenerator.h
    T1DGatingConverter.h
)
set(GATE_HEADERS
    TTreeFormulaGateProcessor.h
    TTrackXYGateProcessor.h
    TPIDGateProcessor.h
    TGateStopProcessor.h
    TGateObject.h
    TGateInfo.h
    TGateArrayInitializer.h
    TGateArray.h
)
set(MWDC_HEADERS
    TMWDCTrackingResult.h
    TMWDCTrackingProcessor.h
    TMWDCPlaneProcessor.h
    TMWDCPlaneInfo.h
    TMWDCPlaneDLCorrectionProcessor.h
    TMWDCPlaneCalibrationTask.h
    TMWDCParameter.h
    TMWDCHitData.h
    TMWDCDriftLengthCorrectionProcessor.h
    TMWDCCalibrationProcessor.h
    TGMWDCConfig.h
    TGMWDCCalibration.h
)
set(PLASTIC_HEADERS
    TTwoSidedPlasticData.h
    TTwoSidedPlasticCalibrationProcessor.h
)
set(PPAC_HEADERS
    TPPACTrackingProcessor.h
    TPPACProcessor.h
    TPPACParameter.h
    TPPACData.h
)
set(RCNP_HEADERS
    TVDCTrackingResult.h
    TVDCTrackingProcessor_las.h
    TVDCTrackingProcessor.h
    TVDCTrackingProcessor_gr.h
    TVDCPlaneProcessor.h
    TVDCClusterizationProcessor.h
    TVDCCluster.h
    TRCNPRunInfo.h
    TRCNPEventStore_ts.h
    TRCNPEventHeader.h
)
set(RCNPDECODER_HEADERS
    TModuleDecoderV830.h
    TModuleDecoderV1190_rcnp.h
    TModuleDecoderTS.h
    TModuleDecoderRCNP.h
    TModuleDecoderMXDC32_rcnp.h
    TModuleDecoderFERET.h
    TModuleDecoderFERA.h
    TModuleDecoder3377.h
)
set(SCALER_HEADERS
    TCatScalerMappingProcessor.h
)
set(SH04_HEADERS
    TTrackProjectionProcessor.h
    TSMWDCTrackingProcessor.h
    TSMWDCDriftLengthCalibrator.h
    TSlewCorrectionProcessor.h
    TS1PIDProcessor.h
    TResidualPIDProcessor.h
    TRecoilTOFProcessor.h
    TRecoilProcessor.h
    TReactionVertexProcessor.h
    TParticleIdentifier.h
    TP2PKinematicsProcessor.h
    TP2PKinematicsData.h
    TNyokiTimingProcessor.h
    TNyokiGateProcessor.h
    TNyokiChargeCorrectionProcessor.h
    TDetectorGeometry.h
    TBeamZProcessor.h
    TBeamZ.h
    TBeamParameter.h
)
set(HEADERS
    TWalkCorrectionProcessor.h
    TTimingSubtractionProcessor.h
    TTimingChargePositionMappingProcessor.h
    TTimingChargeMappingProcessor.h
    TTimingChargeCalibrationProcessor.h
    TTimeReferenceProcessor.h
    TSeparateOutputProcessor.h
    TRIDFEventShifter.h
    TMultiHitCombinator.h
    TMultiHitArray.h
    TMultiFileParameterArrayLoader.h
    TExtractOutputProcessor.h
    TDataCalibrationProcessor.h
    TMergeTrackProcessor.h
    #TCatPosition.h
    constant.h
)
set(DIC_HEADERS ${HEADERS} ${CAT_HEADERS} ${COIN_HEADERS} ${COMMAND_HEADERS} ${CONT_HEADERS} ${CONVERTER_HEADERS}
    ${GATE_HEADERS} ${PLASTIC_HEADERS} ${MWDC_HEADERS} ${PPAC_HEADERS} ${SCALER_HEADERS}
    ${RCNP_HEADERS} ${RCNPDECODER_HEADERS} ${SH04_HEADERS})

list(TRANSFORM CAT_HEADERS PREPEND "cat/")
list(TRANSFORM COIN_HEADERS PREPEND "coin/")
list(TRANSFORM COMMAND_HEADERS PREPEND "command/")
list(TRANSFORM CONT_HEADERS PREPEND "cont/")
list(TRANSFORM CONVERTER_HEADERS PREPEND "converter/")
list(TRANSFORM GATE_HEADERS PREPEND "gate/")
list(TRANSFORM PLASTIC_HEADERS PREPEND "plastic/")
list(TRANSFORM PPAC_HEADERS PREPEND "ppac/")
list(TRANSFORM MWDC_HEADERS PREPEND "mwdc/")
list(TRANSFORM SCALER_HEADERS PREPEND "scaler/")
list(TRANSFORM RCNP_HEADERS PREPEND "rcnp/")
list(TRANSFORM RCNPDECODER_HEADERS PREPEND "rcnp_decoder/")
list(TRANSFORM SH04_HEADERS PREPEND "sh04/")

set(PUB_HEADERS ${HEADERS} ${CAT_HEADERS} ${COIN_HEADERS} ${COMMAND_HEADERS} ${CONT_HEADERS} ${CONVERTER_HEADERS}
    ${GATE_HEADERS} ${PLASTIC_HEADERS} ${MWDC_HEADERS} ${PPAC_HEADERS} ${SCALER_HEADERS}
    ${RCNP_HEADERS} ${RCNPDECODER_HEADERS} ${SH04_HEADERS})

art_lib(artshare "${OBJECTS}" "${DIC_HEADERS}" "linkdef_user.h" "${PUB_HEADERS}")
