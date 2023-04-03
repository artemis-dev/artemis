# target
LIBNAME = libuser

# subdirectories
VPATH = hist cat coin command converter cont decoder mwdc plastic ppac scaler gate sh04 rcnp rcnp_decoder

# object to be compiled should be listed without directory
# main
OBJ+= TSeparateOutputProcessor.o
OBJ+= TExtractOutputProcessor.o
OBJ+= TWalkConverter.o
OBJ+= TWalkCorrectionProcessor.o
# cat
OBJ+= TSampleCalibratorProcessor.o
OBJ+= TCatAtMappingProcessor.o
OBJ+= TCatReadoutPadConditionProcessor.o
OBJ+= TCatAtPedestalCalculator.o
OBJ+= TCatAtPedestalProcessor.o
OBJ+= TCatAtHitPatternProcessor.o
OBJ+= TCatAtSiHitPatternProcessor.o
OBJ+= TCatPulseBitCorrectionProcessor.o
OBJ+= TCatAtSpillCounter.o
OBJ+= TCatAtPedestalSubtractionProcessor.o
OBJ+= TCatSiCommonNoiseSubtractionProcessor.o
OBJ+= TCatSiPedestalSubtractionProcessor.o
OBJ+= TCatSiHitFindProcessor.o
OBJ+= TPulseUnfoldHighPassProcessor.o
OBJ+= TPulseUnfoldLowPassProcessor.o
OBJ+= TPulseFinder.o
OBJ+= THitFinder.o
OBJ+= TPulseMovingAverageProcessor.o
# OBJ+= TCatV1740AdcCorrectionProcessor.o
OBJ+= TCatSumSampleProcessor.o
OBJ+= TCatDiffSampleProcessor.o
OBJ+= TCatGausSampleProcessor.o
# OBJ+= TCatSiHitPatternProcessor.o
# cont
OBJ+= TGenericData.o
OBJ+= TTimingData.o
OBJ+= TChargeData.o
OBJ+= TTimingChargeData.o
OBJ+= TTimingChargePositionData.o
OBJ+= TCatFadcRawData.o
OBJ+= TCatScalerData.o
#OBJ+= TCatPulseShape.o
# TODO: remove two lines since they are moved to artemis main source tree
#OBJ+= TCatReadoutPad.o
#OBJ+= TCatReadoutPadArray.o
OBJ+= TTrack.o
OBJ+= TTimeDifference.o
# decoder
# scaler
OBJ+= TCatScalerMappingProcessor.o

#	TCatTimingCutProcessor.o
# converter
OBJ += TConverterUtil.o
OBJ += TConverterBase.o
OBJ += TAffineConverter.o
OBJ += TAffineConverterArrayGenerator.o
OBJ += TPolynomialConverter.o
OBJ += TPolynomialConverterGenerator.o
OBJ += TPolynomialConverterArrayGenerator.o
OBJ += TMonotoneTableConverter.o
OBJ += TMonotoneTableConverterGenerator.o
OBJ += T1DGatingConverter.o
OBJ += TSplineConverter.o
OBJ += TMultiFileParameterArrayLoader.o
OBJ += TAffineConverterCombinator.o
# gate
OBJ += TGateArray.o
OBJ += TGateObject.o
OBJ += TGateInfo.o
OBJ += TGateArrayInitializer.o
OBJ += TPIDGateProcessor.o
OBJ += TTrackXYGateProcessor.o
OBJ += TGateStopProcessor.o
OBJ += TNyokiGateProcessor.o
OBJ += TTreeFormulaGateProcessor.o
# plastic
OBJ += TTwoSidedPlasticData.o
OBJ += TTwoSidedPlasticCalibrationProcessor.o
# ppac
OBJ += TPPACData.o
OBJ += TPPACProcessor.o
OBJ += TPPACParameter.o
OBJ += TPPACTrackingProcessor.o
# mwdc
OBJ += TMWDCPlaneInfo.o
OBJ += TMWDCHitData.o
OBJ += TMWDCTrackingResult.o
OBJ += TMWDCPlaneProcessor.o
OBJ += TMWDCCalibrationProcessor.o
OBJ += TMWDCTrackingProcessor.o
OBJ += TMWDCParameter.o
OBJ += TMWDCPlaneCalibrationTask.o
OBJ += TGMWDCCalibration.o
OBJ += TGMWDCConfig.o
OBJ += TMWDCDriftLengthCorrectionProcessor.o
OBJ += TMWDCPlaneDLCorrectionProcessor.o
# others
OBJ += TTimingChargeMappingProcessor.o
OBJ += TTimingChargePositionMappingProcessor.o
OBJ += TTimeReferenceProcessor.o
OBJ += TCoinRegMappingProcessor.o
OBJ += TCatRecoilFinder.o
OBJ += TTimingChargeCalibrationProcessor.o
OBJ += TTimingSubtractionProcessor.o
OBJ += TMultiHitArray.o
OBJ += TMultiHitCombinator.o
OBJ += TRIDFEventShifter.o
OBJ += TDataCalibrationProcessor.o
OBJ += TCoinRegGateProcessor.o
OBJ += TCoinRegData.o
# sh04
OBJ += TDetectorGeometry.o
OBJ += TRecoilProcessor.o
OBJ += TRecoilTOFProcessor.o
OBJ += TP2PKinematicsData.o
OBJ += TP2PKinematicsProcessor.o
OBJ += TSMWDCTrackingProcessor.o
OBJ += TSMWDCDriftLengthCalibrator.o
OBJ += TBeamParameter.o
OBJ += TBeamZ.o
OBJ += TBeamZProcessor.o
OBJ += TReactionVertexProcessor.o
OBJ += TSlewCorrectionProcessor.o
OBJ += TNyokiTimingProcessor.o
OBJ += TNyokiChargeCorrectionProcessor.o
OBJ += TTrackProjectionProcessor.o
OBJ += TParticleIdentifier.o
OBJ += TResidualPIDProcessor.o
OBJ += TS1PIDProcessor.o
# test
OBJ += TFixedNumberParameter.o
# command
OBJ += TCmdMWDCCalib.o
OBJ += TCmdMWDCConfig.o
OBJ += TCmdFiga.o

# rcnp by Y.HIJIKATA
OBJ += TRCNPEventHeader.o
OBJ += TRCNPEventStore_ts.o
OBJ += TRCNPRunInfo.o
OBJ += TVDCCluster.o
OBJ += TVDCClusterizationProcessor.o
OBJ += TVDCPlaneProcessor.o
OBJ += TVDCTrackingProcessor.o
OBJ += TVDCTrackingProcessor_gr.o
OBJ += TVDCTrackingProcessor_las.o
OBJ += TVDCTrackingResult.o

# rcnp_decoder by Y.HIJIKATA
OBJ += TModuleDecoderRCNP.o
OBJ += TModuleDecoder3377.o
OBJ += TModuleDecoderFERA.o
OBJ += TModuleDecoderFERET.o
OBJ += TModuleDecoderTS.o
OBJ += TModuleDecoderV830.o
OBJ += TModuleDecoderV1190_rcnp.o
OBJ += TModuleDecoderMXDC32_rcnp.o

# check architecture
#include $(ROOTSYS)/etc/root/Makefile.arch

UNAME = $(shell uname)
ROOTVER := $(shell root-config --version | cut -c 1)
ifeq ($(UNAME),Darwin)
SOEXT = dylib
#SOEXT = so
SOFLAG = -Wl,-undefined -Wl,dynamic_lookup -dynamiclib -install_name @rpath/$(LIBNAME).$(SOEXT)
#SOFLAG = -dynamiclib
#SOFLAG = -Wl,-undefined -Wl,dynamic_lookup -install_name \
#	$(WORKDIR)/processors/$(LIBNAME).dylib -dynamiclib
else
SOFLAG = -shared -Wl,-soname,$(LIBNAME).so
SOEXT = so
endif

TARGET = $(LIBNAME).$(SOEXT)

# local include directories
INCLUDE = $(addprefix -I, $(VPATH))
# depends
DEPDIR = .deps
DEPENDS = $(addprefix $(DEPDIR)/, $(notdir $(OBJ:.o=.d)))
# object
OBJDIR = .objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

# include files to be installed
HDR = $(OBJ:.o=.h) constant.h

DICTSRC = $(LIBNAME)_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)
DICTPCM = $(DICTSRC:.cc=_rdict.pcm)

CXX = $(shell artemis-config --cxx)

WARNING = -Wall -Wextra

CXXFLAGS = -O2 $(WARNING) -I. $(INCLUDE) `artemis-config --cflags`  -fPIC `root-config --cflags` -g
LDFLAGS = `artemis-config --libs` `root-config --libs` -lTreePlayer
PREFIX := $(shell artemis-config --prefix)
LIBDIR := $(PREFIX)/lib
INCDIR := $(PREFIX)/include

HEADERS = $(foreach file, $(HDR), $(shell find . -name $(file)))


all: $(TARGET)
	@echo compile with ROOT$(ROOTVER)

ifeq ($(ROOTVER),5)
install: install-root5
uninstall: uninstall-root5
else
install: install-root6
uninstall: uninstall-root6
endif

.PHONY: all clean

$(TARGET): $(OBJECTS) $(DICTOBJ)
	@echo `uname`
	$(CXX) $(SOFLAG) -o $@ $^ $(LDFLAGS)

-include $(DEPENDS)

$(DEPDIR)/%.d: %.cc
	@mkdir -p $(DEPDIR)
	g++ -M $(CXXFLAGS) $^ > $@
	@mv -f $@ $@.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $@.tmp > $@
	@sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $@
	@rm -f $@.tmp

$(DICTSRC): $(HDR) linkdef_user.h
	rootcint -f $@ -c -I. $(INCLUDE) `artemis-config --cflags`  -I$(ROOTSYS)/include $^

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f  $(DEPDIR)/*.d $(OBJDIR)/*.o $(LIBNAME).$(SOEXT) $(DICTOBJ) $(DICTSRC) $(DICTHDR)
	rmdir $(OBJDIR) $(DEPDIR)


install-root5: $(TARGET) $(HEADERS)
	test -f $(TARGET) && install -c -m 644 $(TARGET) $(LIBDIR)
	install -c -m 644 $(HEADERS) $(INCDIR)
install-root6: $(DICTPCM) $(TARGET) $(HEADERS)
	test -f $(DICTPCM) && install -c -m 644 $(DICTPCM) $(LIBDIR)
	test -f $(TARGET) && install -c -m 644 $(TARGET) $(LIBDIR)
	install -c -m 644 $(HEADERS) $(INCDIR)


uninstall-root5:  $(TARGET) $(HEADERS)
	cd $(LIBDIR) && rm -f $(TARGET)
	cd $(INCDIR) && rm -f $(HDR)
uninstall-root6: $(DICTPCM) $(TARGET) $(HEADERS)
	cd $(LIBDIR) && rm -f $(DICTPCM)
	cd $(LIBDIR) && rm -f $(TARGET)
	cd $(INCDIR) && rm -f $(HDR)
