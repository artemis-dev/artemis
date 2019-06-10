SUBSRC  =
SUBSRC += TCatReadoutPad.cc
SUBSRC += TCatReadoutPadArray.cc
SUBSRC += TCatPulseShape.cc
SUBSRC += TSimpleData.cc
SUBSRC += ICharge.cc
SUBSRC += ITiming.cc
SUBSRC += ITrack.cc
SUBSRC += IPosition.cc
SUBSRC += TDetectorGeometryInfo.cc
SUBSRC += TNArray.cc


SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h)
# TCatTypes.h


DICTSRC = artcont_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign

MYINCLUDES = -I$(INCDIR) -I$(ROOTSYS)/include 
CXXFLAGS = -fPIC  -I$(INCDIR) $(shell root-config --cflags) -Wall -O2
#CXXFLAGS += -pg
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libartcont.la
libartcont_la_SOURCES = $(SUBSRC) $(DICTSRC)
libartcont_la_DEPENDENCIES = .header
# libartcont_la_LIBADD = $(SUBOBJ)
libartcont_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) artcont_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c -p $(DEFAULT_INCLUDES) $(MYINCLUDES) $^

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
