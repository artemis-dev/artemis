SUBSRC = \
	TArtRint.cc

SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


AUTOMAKE_OPTIONS = foreign

LIBCATCORE = ../core/.libs/libcatcore.$(SOEXT)
LIBCATCMD = ../commands/.libs/libcatcmd.$(SOEXT)

INCDIR = ../../include


CXXFLAGS = -Iinclude -I$(INCDIR)  $(shell root-config --cflags) 
LDFLAGS = -O -L../commands/.libs -lcatcmd \
	-L../loop/hist/.libs -larthist \
	-L../loop/param/.libs -lartparam   \
	-L../loop/ridf/.libs -lartridf \
	-L../loop/rdf/.libs -lartrdf \
	-L../loop/decoder/.libs -lartdecoder  \
	-L../loop/.libs -lcatloop \
	-L../core/.libs -lcatcore \
	-L../mc/.libs -lcatmc $(shell root-config --glibs) -lTreePlayer -lyaml-cpp 


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
