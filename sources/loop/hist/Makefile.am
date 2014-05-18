SUBDIRS = 

SUBSRC = 
SUBSRC+=TAttCut.cc
SUBSRC+=TAttTreeProj.cc
SUBSRC+=TAxisTreeProj.cc
SUBSRC+=TH1FTreeProj.cc
SUBSRC+=TH2FTreeProj.cc
SUBSRC+=TH3FTreeProj.cc
SUBSRC+=TTreeProjection.cc
SUBSRC+=TTreeProjectionProcessor.cc
SUBSRC+=TTreeProjGroup.cc


SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = arthist_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 

INCDIR = ../../../include

AUTOMAKE_OPTIONS = foreign
CXXFLAGS = -fPIC  -I.. -I$(INCDIR) $(shell root-config --cflags)  -Wall -O2
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS += @YAML_CPP_INCLUDE@
LDFLAGS  += @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libarthist.la
libarthist_la_SOURCES = $(SUBSRC) $(DICTSRC)
libarthist_la_DEPENDENCIES = header 
# libarthist_la_LIBADD = $(SUBOBJ)
libarthist_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) arthist_linkdef.h
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
