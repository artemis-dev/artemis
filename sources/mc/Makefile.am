SUBSRC = \
	TArtEventCollections.cc \
	TArtParticle.cc \
	TArtAtomicMassTable.cc \
	TCatTwoBodyKinematics.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) TArtSystemOfUnit.h


DICTSRC = catmc_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h) 

OPTHDR = 
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(INCDIR) -I$(ROOTSYS)/include 
CXXFLAGS = -fPIC  $(shell root-config --cflags) -I$(INCDIR) -Wall -O2
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatmc.la
libcatmc_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatmc_la_DEPENDENCIES = header
# libcatmc_la_LIBADD = $(SUBOBJ)
libcatmc_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catmc_linkdef.h
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
