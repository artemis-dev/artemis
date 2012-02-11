SUBSRC = \
	TArtRint.cc 
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


AUTOMAKE_OPTIONS = foreign

DICTSRC = catmain_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h) 

LIBCATCORE = ../core/.libs/libcatcore.$(SOEXT)
LIBCATCMD = ../commands/.libs/libcatcmd.$(SOEXT)

INCDIR = ../../include

INCLUDES = -I../core/ -I$(ROOTSYS)/include -I../commands/
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(INCDIR) -I$(TARTSYS)/include
LDFLAGS = $(shell root-config --glibs) -L../commands/.libs -lcatcmd\
	-L../core/.libs -lcatcore -L$(TARTSYS)/lib -lanacore

include_HEADERS = $(SUBHDR) $(SUBHDRF)
bin_PROGRAMS = anaroot
anaroot_SOURCES = $(SUBSRC) $(DICTSRC) main.cc
anaroot_DEPENDENCIES = $(LIBCATCORE) $(LIBCATCMD)
anaroot_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catmain_linkdef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

header: $(include_HEADERS)
	@mkdir -p $(INCDIR)
	cp -a $(include_HEADERS) $(INCDIR)

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
