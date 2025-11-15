# target
LIBNAME = libCAT

VPATH = hist cat coin command converter cont decoder mwdc plastic ppac scaler gate sh04 

OBJ += TModuleDecoderA3100.o
OBJ += TModuleDecoderA3100TriggeredList.o
OBJ += TModuleDecoderHINP.o
OBJ += TSimpleDataMappingProcessor.o
OBJ += TChargeValidateProcessor.o
OBJ += TCatGainEvaluationProcessor.o
OBJ += TCmdXsta.o
OBJ+= TCatAtSaveHitPatternProcessor.o
OBJ+= TCatAtEventSelectionByRecoilHitPattern.o
OBJ+= TCatAtRoughEventCounter.o
OBJ+= TCatAtPedestalSubtractTester.o
OBJ+= TPulseUnfoldHighPassTester.o
OBJ+= TSampleCalibratorTester.o
OBJ+= TPulseFindTester.o
OBJ+= TGraphDrawer.o
OBJ+= TPulseDrawer.o
OBJ+= TModuleDecoderSIS3301.o
OBJ+= TMonitorMappingProcessor.o
OBJ+= TCatBeamTrackSimulator.o
OBJ+= TModuleDecoderV1740_mod.o
OBJ+= TCatAtSaveChargePatternProcessor.o
OBJ += TCatHitFinderByCharge.o
OBJ += THogeProcessor.o
# OBJ += THogehogeProcessor.o
OBJ += TCatBeamXProcessor.o
OBJ += TCatRefineRecoilClusterProcessor.o
OBJ += TEventSkipProcessor.o
OBJ += TCatTrackResultPulse.o
OBJ += TCatAtClusteringProcessor.o
#OBJ += TCatAtCountNumberOfCluster.o
OBJ += TCoinRegUnGateProcessor.o
OBJ += TBetaDecomposeProcessor.o
OBJ += TBrhoReconstructProcessor.o
OBJ += TPIDProcessor.o
OBJ += TCatTPCRecoilCandidateSelectProcessor.o
OBJ += TCatTPCRecoilCandidateRescanByDistanceProcessor.o
#OBJ += TCatCentroidTrackingProcessor.o
OBJ += TEventDisplayProcessorHelper.o
OBJ += TRangeTableHelper.o
OBJ += TCatTrackMinuitHelper.o
OBJ += TCatTrackDiffusedMinuitHelper.o
OBJ += TCatTrackingWithDiffusedChargeProcessor.o
OBJ += TCatTrackResult.o
OBJ += TCatTrackResultDisplayProcessor.o
OBJ += TCatMissingMassProcessor.o
OBJ += TCatAtBeamInRecoilRegionCounter.o
OBJ += TCatAtSaveMaxSamplePatternProcessor.o
OBJ += TCatRegionHitsDivider.o
OBJ += TPPACAnodeTimingProcessor.o
OBJ += TPPACProcessorQTC.o
OBJ += TRawDataTriggeredList.o
OBJ += TCatDiffusedChargeResponseHelper.o
OBJ += TCatFunctionChargeResidual.o
OBJ += TRangeTableConditionProcessor.o
OBJ += TCatRecoilEventGenerator.o
OBJ += TAffineConverterConditionProcessor.o
OBJ += TCatTpcHitClusterProcessor.o
OBJ += TCatTpcTrack.o
OBJ += TCatBeamHitsFinderWithRecoil.o
OBJ += TParticleGenerateProcessor.o
OBJ += TUpdateParticleMomentumProcessor.o
OBJ += TMissingMassProcessor.o
OBJ += TTimingChargeTriggeredListMappingProcessor.o
OBJ += TArtemisCanvas.o
OBJ += TPulsePoleZeroCancellationProcessor.o
OBJ += TCatTpcTrackUpdateHelper.o
OBJ += TNArrayLoader.o
OBJ += TTrackGenerator.o
OBJ += TEventInfoGenerator.o
OBJ += TTpcEventGenerator.o
OBJ += TArtMathUtil.o
OBJ += TPIDwSinglePlaneProcessor.o
OBJ += TPIDwSinglePPACPlaneProcessor.o
OBJ += TGenerateParticleFromPID.o
OBJ += TUpdateParticleMomentumByBrho.o
OBJ += TUpdateParticleMomentumByTable.o
OBJ += TCatTrackFittingWithSiliconProcessor.o
OBJ += TSiliconEventGenerator.o
OBJ += TEnergyCorrectionInMaterial.o
OBJ += TDetectorGeometryUpdateProcessor.o
# ribf113
OBJ += TCatGlastPositionConditionProcessor.o
OBJ += TCatTrackFindWithSiliconProcessor.o
OBJ += TChargeRiseTimeCompensateProcessor.o
OBJ += TTimestampCalibrationProcessor.o
OBJ += TCatClusterSumProcessor.o

# srppac related
OBJ+= TSRPPACPlaneData.o
OBJ+= TSRPPACParameter.o
OBJ+= TSRPPACPlaneProcessor.o
OBJ+= TSRPPACPlaneDqdxProcessor.o
OBJ+= TSRPPACPlaneSideDqdxProcessor.o
OBJ+= TSRPPACCalibrationProcessor.o
OBJ+= TSRPPACPlaneCalibrationTask.o
#OBJ+= TGSRPPACConfig.o
OBJ+= TGSRPPACCalibration.o
OBJ+= TTpcStaticGasPropertyConditionProcessor.o
OBJ+= TResponseFunctionNArray.o
OBJ+= TCatEvalTpcChargeResolution.o
OBJ+= TCatEvalGainShiftProcessor.o
OBJ+= TArtLocalUtil.o
OBJ+= TUnsharpMask.o

CXX=$(shell artemis-config --cxx)
UNAME = $(shell uname)
ifeq ($(UNAME),Darwin)
SOFLAG = -Wl,-undefined -Wl,dynamic_lookup -dynamiclib
SOEXT = so
else
SOFLAG = -shared -Wl,-soname,$(LIBNAME).so
SOEXT = so
endif

TARGET = $(LIBNAME).$(SOEXT)

# local include directories
INCLUDE = $(addprefix -I../share/src/, $(VPATH))
INCLUDE += -I../share/src -I${ART_SHARE_DIR}/include
# depends
DEPDIR = .deps
DEPENDS = $(addprefix $(DEPDIR)/, $(notdir $(OBJ:.o=.d)))
# object
OBJDIR = .objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

HDR = $(OBJ:.o=.h) 

DICTSRC = $(LIBNAME)_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h) 

ARTEMIS_HOME ?= /opt/local


ifeq ($(CXX),icpc)
CXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags` -fPIC `root-config --cflags` -qopenmp
# CXXFLAGS += -g 
DICTCXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags`  #`root-config --cflags` 
LDFLAGS = `artemis-config --libs`  -L../share/src -L${ART_SHARE_DIR}/lib -luser `root-config --libs` -lTreePlayer -lGeom  -qopenmp
else
ifeq ($(CXX),mpiicpc)
CXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags` -fPIC `root-config --cflags` -qopenmp -DHAVE_MPI_H -DUSE_MPI
# CXXFLAGS += -g n
DICTCXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags`  #`root-config --cflags` 
LDFLAGS = `artemis-config --libs`  -L../share/src -L${ART_SHARE_DIR}/lib -luser `root-config --libs` -lTreePlayer -lGeom  -qopenmp
else
CXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags` -fPIC `root-config --cflags` 
CXXFLAGS += -g 
DICTCXXFLAGS = -O2 -Wall -I. $(INCLUDE) `artemis-config --cflags` 
LDFLAGS = `artemis-config --libs`  -L../share/src -L${ART_SHARE_DIR}/lib -luser `root-config --libs` -lTreePlayer -lGeom
endif
endif


CXXFLAGS += -g
all: $(TARGET)
.PHONY: all clean

$(TARGET): $(OBJECTS) $(DICTOBJ)
	@echo `uname`
	$(CXX) $(SOFLAG) -o $@ $^ $(LDFLAGS)

-include $(DEPENDS)

$(DEPDIR)/%.d: %.cc
	@mkdir -p $(DEPDIR)
	$(CXX) -M $(CXXFLAGS) $^ > $@
	@mv -f $@ $@.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $@.tmp > $@
	@sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $@
	@rm -f $@.tmp

$(DICTSRC): $(HDR) linkdef_user.h
	rootcint -f $@ -c $(DICTCXXFLAGS) $^

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f  $(DEPDIR)/*.d $(OBJDIR)/*.o $(LIBNAME).$(SOEXT) $(DICTOBJ) $(DICTSRC) $(DICTHDR)
	rmdir $(OBJDIR) $(DEPDIR)
