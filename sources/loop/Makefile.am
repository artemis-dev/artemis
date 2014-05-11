SUBDIRS = decoder ridf

SUBSRC = 
SUBSRC+=TConditionBit.cc 
SUBSRC+=TLoop.cc 
SUBSRC+=TEventObject.cc 
SUBSRC+=TEventCollection.cc 
SUBSRC+=TLoopManager.cc 
SUBSRC+=TLoopControl.cc 
SUBSRC+=TProcessor.cc 
SUBSRC+=TParameterStrings.cc 
SUBSRC+=TTimerProcessor.cc 
SUBSRC+=TCounterEventStore.cc 
SUBSRC+=TRandomNumberEventStore.cc 
SUBSRC+=TRandomNumberProcessor.cc 
SUBSRC+=TTreeEventStore.cc 
SUBSRC+=TOutputTreeProcessor.cc 
SUBSRC+=TRawDataObject.cc 
SUBSRC+=TSegmentedData.cc 
SUBSRC+=TCategorizedData.cc 
SUBSRC+=TFileDataSource.cc 
SUBSRC+=TMapTable.cc 
SUBSRC+=TModuleDecoder.cc 
SUBSRC+=TModuleDecoderFactory.cc 
SUBSRC+=TRawDataSimple.cc 
SUBSRC+=TRawTiming.cc 
SUBSRC+=TRawTimingWithEdge.cc 
SUBSRC+=TRawDataPair.cc 
SUBSRC+=TRunInfo.cc
SUBSRC+=TEventHeader.cc
SUBSRC+=TMappingProcessor.cc
SUBSRC+=TSharedMemoryDataSource.cc



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
libcatloop_la_DEPENDENCIES = header 
# libcatloop_la_LIBADD = $(SUBOBJ)
libcatloop_la_LDFLAGS = $(LDFLAGS)
libcatloop_la_LIBADD   = ridf/libartridf.la decoder/libartdecoder.la

$(DICTSRC): $(SUBHDR) catloop_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c $(CXXFLAGS)  $^

header: $(include_HEADERS)
	@echo Installing header to $(INCDIR)
	@mkdir -p $(INCDIR)
	@cp -a $(include_HEADERS) $(INCDIR)


.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
