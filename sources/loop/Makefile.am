


SUBDIRS = @LOOP_SUB_DIR@

SUBSRC =
SUBSRC+=TCategorizedData.cc 
SUBSRC+=TConditionBit.cc 
SUBSRC+=TCounterEventStore.cc 
SUBSRC+=TEventCollection.cc 
SUBSRC+=TEventHeader.cc
SUBSRC+=TEventObject.cc 
SUBSRC+=TFileDataSource.cc 
SUBSRC+=TFileDataSourceGZ.cc 
SUBSRC+=TLoop.cc 
SUBSRC+=TLoopControl.cc 
SUBSRC+=TLoopManager.cc 
SUBSRC+=TMapTable.cc 
SUBSRC+=TMappingProcessor.cc
SUBSRC+=TModuleDecoder.cc 
SUBSRC+=TModuleDecoderFactory.cc
SUBSRC+=TModuleInfo.cc
SUBSRC+=TModuleType.cc
SUBSRC+=TOutputTreeProcessor.cc 
SUBSRC+=TProcessor.cc 
SUBSRC+=TRandomNumberEventStore.cc 
SUBSRC+=TRandomNumberProcessor.cc 
SUBSRC+=TRawDataObject.cc 
SUBSRC+=TRawDataSimple.cc 
SUBSRC+=TRawTiming.cc 
SUBSRC+=TRawTimingWithEdge.cc 
SUBSRC+=TSegmentCheckProcessor.cc
SUBSRC+=TSegmentedData.cc
SUBSRC+=TSegmentInfo.cc
SUBSRC+=TSharedMemoryDataSource.cc
SUBSRC+=TTimerProcessor.cc 
SUBSRC+=TTreeEventStore.cc 
SUBSRC+=IProcessorHelper.cc
SUBSRC+=TScalerData.cc
SUBSRC+=TTimestampEventList.cc
SUBSRC+=TTimestampEventReconstructor.cc
SUBSRC+=TScalerTimestampProcessor.cc
SUBSRC+=THeaderTimestampProcessor.cc
SUBSRC+=TScalerMappingProcessor.cc
SUBSRC+=TObjectLoader.cc
SUBSRC+=TAsynchronousRunEndProcessor.cc
SUBSRC+=TPulseShapeTimestampProcessor.cc
SUBSRC+=TTimeValidator.cc
SUBSRC+=TChargeValidator.cc
SUBSRC+=TTimingTimestampAdapter.cc
SUBSRC+=TTimestampValidator.cc
SUBSRC+= TGainShiftCorrectionProcessor.cc
SUBSRC+=IEventStore.cc
SUBSRC+=TFormulaData.cc
SUBSRC+=TFormulaDataGenerator.cc
SUBSRC+=TEventNumberConditionProcessor.cc
SUBSRC+=TArtTree.cc


OPTSRC = 
OPTSRC+=TAttParameter.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 
OPTOBJ = $(OPTSRC:.cc=.o)

DICTSRC = catloop_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = $(OPTSRC:.cc=.h)
OPTHDR+= TDataSource.h 

INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC  -I$(INCDIR) $(shell root-config --cflags)  -Wall -O2 -g
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS += @YAML_CPP_INCLUDE@
LDFLAGS  += @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatloop.la
libcatloop_la_SOURCES = $(SUBSRC) $(OPTSRC) $(DICTSRC)
libcatloop_la_DEPENDENCIES = .header 
# libcatloop_la_LIBADD = $(SUBOBJ)
libcatloop_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) catloop_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c -p -I$(INCDIR) $^

.header: $(include_HEADERS)
	@echo Installing header to $(INCDIR)
	@mkdir -p $(INCDIR)
	@cp -a $(include_HEADERS) $(INCDIR)
	@touch .header

if HAVE_ROOT_PCM
DICTPCM = $(DICTSRC:.cc=_rdict.pcm)
install-exec-local:
	@echo Copying PCM file to $(libdir)
	$(INSTALL) $(DICTPCM) $(libdir)
uninstall-local:
	rm -f $(libdir)/$(DICTPCM)
endif

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict* .header

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo .header
	- for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean); done
