SUBSRC = \
	TModuleDecoderV1190.cc \
	TRawDataV1190.cc \
	TModuleDecoderV1740.cc \
	TModuleDecoderSkip.cc \
	TModuleDecoderV767.cc \
	TModuleDecoderV7XX.cc \
	TModuleDecoderFixed.cc \
	TRawDataFadc.cc 



SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 

SUBHDR += TRawDataV767.h

DICTSRC = artdecoder_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC -I.. -I$(INCDIR) $(shell root-config --cflags)  -Wall -O2
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS+= @YAML_CPP_INCLUDE@
LDFLAGS+= @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

noinst_LTLIBRARIES = libartdecoder.la
libartdecoder_la_SOURCES = $(SUBSRC) $(DICTSRC)
libartdecoder_la_DEPENDENCIES = header
# libartdecoder_la_LIBADD = $(SUBOBJ)
libartdecoder_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) artdecoder_linkdef.h
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
