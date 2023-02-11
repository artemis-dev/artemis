SUBSRC = \
	TArtRint.cc \
	main.cc


SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(filter-out main.h, $(SUBSRC:.cc=.h)) 


AUTOMAKE_OPTIONS = foreign

LIBCATCORE = ../core/.libs/libcatcore.$(SOEXT)
LIBCATCMD = ../commands/.libs/libcatcmd.$(SOEXT)

INCDIR = ../../include


CXXFLAGS = $(DEFS) -I../..
CXXFLAGS+= -Iinclude -I$(INCDIR)  $(shell root-config --cflags) 
LDFLAGS = -O \
	-L../mc/.libs -lcatmc \
        -L../commands/.libs -lcatcmd \
	-L../loop/.libs -lcatloop \
	-L../loop/hist/.libs -larthist \
	-L../loop/rdf/.libs -lartrdf \
	-L../loop/ridf/.libs -lartridf \
	-L../loop/decoder/.libs -lartdecoder  \
	-L../loop/streaming/.libs -lartstreaming  \
	-L../loop/param/.libs -lartparam   \
	-L../cont/.libs -lartcont \
	-L../core/.libs -lcatcore \
	$(shell root-config --glibs) -lTreePlayer -lyaml-cpp 
LDFLAGS+=@ARTGET_LIB@
LDFLAGS+=@GET_DECODER_LIB@

include_HEADERS = $(SUBHDR) $(SUBHDRF)
bin_PROGRAMS = artemis 
artemis_SOURCES = $(SUBSRC) 
artemis_DEPENDENCIES = $(LIBCATCORE) $(LIBCATCMD)
artemis_LDFLAGS = $(LDFLAGS)

bin_SCRIPTS = artemis-config

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo artemis artemis-config 
