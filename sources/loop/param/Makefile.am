SUBDIRS = 

SUBSRC = 
SUBSRC+=TParameterLoader.cc
SUBSRC+=TParameterArrayLoader.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = artparam_dict.cc
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

lib_LTLIBRARIES = libartparam.la
libartparam_la_SOURCES = $(SUBSRC) $(DICTSRC)
libartparam_la_DEPENDENCIES = .header 
# libartparam_la_LIBADD = $(SUBOBJ)
libartparam_la_LDFLAGS = $(LDFLAGS)


$(DICTSRC): $(SUBHDR) artparam_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c -I.. -I$(INCDIR)  $^

.header: $(include_HEADERS)
	@echo Installing header to $(INCDIR)
	@mkdir -p $(INCDIR)
	@cp -a $(include_HEADERS) $(INCDIR)
	@touch .header

if HAVE_ROOT_PCM
DICTPCM = $(DICTSRC:.cc=_rdict.pcm)
install-exec-local:
	@echo Copying PCM file to $(libdir)
	$(INSTALL) $(DICTPCM) $(libdir)
uninstall-local:
	rm -f $(libdir)/$(DICTPCM)
endif

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict* .header

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
