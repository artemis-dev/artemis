SUBSRC = \
	TArtEventCollections.cc \
	TArtParticle.cc \
	TArtAtomicMassTable.cc \
	TCatTwoBodyKinematics.cc \
	TBinaryReactionGenerator.cc \
	TTrackG4.cc \
	TRandomGraph.cc 

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h)


DICTSRC = catmc_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h) 

OPTHDR = 
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign

CXXFLAGS = -fPIC  -I$(INCDIR)  $(shell root-config --cflags) -Wall -O2
LDFLAGS = $(shell root-config --glibs)  -lGeom

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatmc.la
libcatmc_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatmc_la_DEPENDENCIES = .header
# libcatmc_la_LIBADD = $(SUBOBJ)
libcatmc_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catmc_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c $(DEFAULT_INCLUDES) -I$(INCDIR) -I$(ROOTSYS)/include $^

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
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo .header
