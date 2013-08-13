SUBDIRS = decoder ridf

SUBSRC = \
	TConditionBit.cc \
	TLoop.cc \
	TEventObject.cc \
	TEventCollection.cc \
	TLoopManager.cc \
	TLoopControl.cc \
	TProcessor.cc \
	TParameter.cc \
	TParameterStrings.cc \
	TTimerProcessor.cc \
	TCounterEventStore.cc \
	TRandomNumberEventStore.cc \
	TRandomNumberProcessor.cc \
	TTreeEventStore.cc \
	TOutputTreeProcessor.cc \
	TRawDataObject.cc \
	TSegmentedData.cc \
	TCategorizedData.cc \
	TFileDataSource.cc \
	TMapTable.cc \
	TModuleDecoder.cc \
	TModuleDecoderFactory.cc \
	TRawDataSimple.cc \
	TRawTiming.cc \
	TRawTimingWithEdge.cc \
	TRawDataPair.cc



SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = catloop_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = TDataSource.h TTypes.h
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC  $(shell root-config --cflags) -I$(INCDIR)  -Wall -O2
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
