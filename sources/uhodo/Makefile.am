SUBSRC = \
	TCatUHodoPl.cc \
	TCatUHodo.cc \
	TCatUHodoMappingProcessor.cc 
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 

AUTOMAKE_OPTIONS = foreign

DICTSRC = catuhodo_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h) 

INCDIR = ../../include

INCLUDES = -I../../include/ -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(INCDIR) -I$(TARTSYS)/include
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libcatuhodo.la
libcatuhodo_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatuhodo_la_DEPENDENCIES = header
# libcatuhodo_la_LIBADD = $(SUBOBJ)
libcatuhodo_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catuhodo_linkdef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

header: $(include_HEADERS)
	@echo Install header files to $(INCDIR)
	@mkdir -p $(INCDIR)
	@cp -a $(include_HEADERS) $(INCDIR)
	@touch header
.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
