SUBSRC = \
	TCatCmdFactory.cc \
	TCatCmd.cc \
	TCatCmdHelp.cc \
	TCatCmdHt.cc \
	TCatCmdHtp.cc \
	TCatCmdHb.cc \
	TCatCmdHn.cc \
	TCatCmdZone.cc \
	TCatCmdPr.cc \
	TCatCmdPrx.cc \
	TCatCmdPry.cc \
	TCatCmdBn.cc \
	TCatCmdBnx.cc \
	TCatCmdBny.cc \
	TCatCmdSl.cc \
	TCatCmdSly.cc \
	TCatCmdLs.cc \
	TCatCmdCd.cc
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = catcmd_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

OPTHDR = 


AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(TARTSYS)/include \
	-Wall -I../core
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatcmd.la
libcatcmd_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatcmd_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catcmd_linkdef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean: clean-local
	rm -rf $(SUBOBJ) .libs *.lo
