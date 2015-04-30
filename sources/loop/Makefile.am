SUBDIRS = param decoder ridf rdf hist 

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
SUBSRC+=TParameterStrings.cc 
SUBSRC+=TProcessor.cc 
SUBSRC+=TRandomNumberEventStore.cc 
SUBSRC+=TRandomNumberProcessor.cc 
SUBSRC+=TRawDataObject.cc 
SUBSRC+=TRawDataPair.cc 
SUBSRC+=TRawDataSimple.cc 
SUBSRC+=TRawTiming.cc 
SUBSRC+=TRawTimingWithEdge.cc 
SUBSRC+=TRunInfo.cc
SUBSRC+=TSegmentCheckProcessor.cc
SUBSRC+=TSegmentedData.cc
SUBSRC+=TSegmentInfo.cc
SUBSRC+=TSharedMemoryDataSource.cc
SUBSRC+=TTimerProcessor.cc 
SUBSRC+=TTreeEventStore.cc 



SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = catloop_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = TDataSource.h TTypes.h TProcessorParameter.h

INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC  -I$(INCDIR) $(shell root-config --cflags)  -Wall -O2
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS += @YAML_CPP_INCLUDE@
LDFLAGS  += @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatloop.la
libcatloop_la_SOURCES = $(SUBSRC) $(DICTSRC)
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


.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo .header
	- for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean); done
