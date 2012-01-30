SUBSRC = \
	TArtFadcRawDataObject.cc \
	TArtDecoderV1740.cc
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h)

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(TARTSYS)/include
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libcatcore.la
libcatcore_la_SOURCES = \
	TArtFadcRawDataObject.cc \
	TArtDecoderV1740.cc catcore_dict.cc
# libcatcore_la_DEPENDENCIES = $(SUBOBJ)
# libcatcore_la_LIBADD = $(SUBOBJ)
libcatcore_la_LDFLAGS = $(LDFLAGS)

catcore_dict.cc: $(SUBHDR) catcore_linkdef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
