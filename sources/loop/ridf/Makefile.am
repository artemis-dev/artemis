SUBSRC = \
	TRIDFEventStore.cc 


SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = artridf_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC -I.. -I$(INCDIR) $(shell root-config --cflags) -Wall -O2
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS+= @YAML_CPP_INCLUDE@
LDFLAGS+= @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libartridf.la
libartridf_la_SOURCES = $(SUBSRC) $(DICTSRC)
libartridf_la_DEPENDENCIES = header
# libartridf_la_LIBADD = $(SUBOBJ)
libartridf_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) artridf_linkdef.h
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
