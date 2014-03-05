SUBSRC = \
	TArtRint.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


AUTOMAKE_OPTIONS = foreign

LIBCATCORE = ../core/.libs/libcatcore.$(SOEXT)
LIBCATCMD = ../commands/.libs/libcatcmd.$(SOEXT)

INCDIR = ../../include

INCLUDES = -I. -I../core/ -I$(ROOTSYS)/include -I../commands/
CXXFLAGS = -Iinclude $(shell root-config --cflags) -I$(INCDIR)  -O2
LDFLAGS = $(shell root-config --glibs) \
	-L../commands/.libs -lcatcmd \
	-L../loop/.libs -lcatloop \
	-L../core/.libs -lcatcore \
	-L../mc/.libs -lcatmc -lyaml-cpp

include_HEADERS = $(SUBHDR) $(SUBHDRF)
bin_PROGRAMS = artemis
artemis_SOURCES = $(SUBSRC) main.cc 
artemis_DEPENDENCIES = $(LIBCATCORE) $(LIBCATCMD)
artemis_LDFLAGS = $(LDFLAGS)

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo artemis
