SUBSRC = \
	TArtFadcRawDataObject.cc \
	TArtDecoderV1740.cc \
	TArtDecoderSIS3610.cc \
	TArtDecoderSIS3820.cc \
	TArtKbhit.cc \
	TCatObject.cc \
	TCatSimpleData.cc \
	TCatHitV1190.cc \
	TCatSortable.cc \
	TCatDetectorArray.cc \
	TCatHistManager.cc \
	TCatPadManager.cc \
	TCatRawDataCategorized.cc \
	TCatStreamBuf.cc \
	TCatOstream.cc \
	TCatObjRefArray.cc\
	TCatParameter.cc \
	TCatParameterStrings.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) TCatTypes.h


DICTSRC = catcore_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(INCDIR) -I$(ROOTSYS)/include
CXXFLAGS = -fPIC  $(shell root-config --cflags) -I$(INCDIR) -I$(TARTSYS)/include -Wall
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatcore.la
libcatcore_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatcore_la_DEPENDENCIES = header
# libcatcore_la_LIBADD = $(SUBOBJ)
libcatcore_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catcore_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

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
