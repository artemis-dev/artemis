SUBSRC = \
	TCatLoop.cc \
	TCatLoopManager.cc \
	TCatLoopWidget.cc \
	TCatProcessor.cc \
	TCatSleepProcessor.cc \
	TCatEventCountProcessor.cc \
	TCatCategorizingProcessor.cc \
	TCatEventCollection.cc \
	TCatEventStore.cc \
	TCatRIDFEventStore.cc \
	TConditionBit.cc \
	TLoop.cc \
	TEventObject.cc \
	TEventCollection.cc \
	TLoopManager.cc \
	TLoopControl.cc \
	TProcessor.cc \
	TParameter.cc \
	TParameterStrings.cc \
	TTimerProcessor.cc



SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = catloop_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 
INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign
INCLUDES = -I$(INCDIR) -I$(ROOTSYS)/include
CXXFLAGS = -fPIC  $(shell root-config --cflags) -I$(INCDIR) -I$(TARTSYS)/include -Wall -I../core -O2
LDFLAGS = $(shell root-config --glibs)

CXXFLAGS+= @YAML_CPP_INCLUDE@
LDFLAGS+= @YAML_CPP_LIB@


include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatloop.la
libcatloop_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatloop_la_DEPENDENCIES = header
# libcatloop_la_LIBADD = $(SUBOBJ)
libcatloop_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catloop_linkdef.h
	@echo Making $(DICTSRC) ...
	rootcint -f $@ -c $(INCLUDES) $(CXXFLAGS)  $^

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
