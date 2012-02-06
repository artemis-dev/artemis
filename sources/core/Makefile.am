SUBSRC = \
	TArtFadcRawDataObject.cc \
	TArtDecoderV1740.cc \
	TArtDecoderSIS3610.cc \
	TArtDecoderSIS3820.cc \
	TArtKbhit.cc \
	TCatObject.cc \
	TCatHitV1190.cc \
	TCatSortable.cc \
	TCatDetectorArray.cc
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) \
	TCatDetectorArray.h

DICTSRC = catcore_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 


AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(TARTSYS)/include \
	-Wall
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatcore.la
libcatcore_la_SOURCES = $(SUBSRC) $(DICTSRC)
#libcatcore_la_DEPENDENCIES = $(SUBOBJ)
# libcatcore_la_LIBADD = $(SUBOBJ)
libcatcore_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catcore_linkdef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
