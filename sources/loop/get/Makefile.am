SUBSRC = \
	TGetEventStore.cc 


SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = artget_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../../include 
GETINC = -I @GET_DECODER_INCLUDE_DIR@

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC -I.. -I$(INCDIR) $(shell root-config --cflags) -Wall -O2
LDFLAGS = $(shell root-config --glibs) @GET_DECODER_LIB@

CXXFLAGS+= @YAML_CPP_INCLUDE@ $(GETINC)
LDFLAGS+= @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libartget.la
libartget_la_SOURCES = $(SUBSRC) $(DICTSRC)
libartget_la_DEPENDENCIES = .header
# libartridf_la_LIBADD = $(SUBOBJ)
libartget_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) artget_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c -I.. -I$(INCDIR)  $^

.header: $(include_HEADERS)
	@echo Installing header to $(INCDIR)
	@mkdir -p $(INCDIR)
	@cp -a $(include_HEADERS) $(INCDIR)
	@touch .header


.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict* .header

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo .header
