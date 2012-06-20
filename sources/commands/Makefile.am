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
	TCatCmdLoopAdd.cc \
	TCatCmdLoopSuspend.cc \
	TCatCmdLoopResume.cc \
	TCatCmdCd.cc
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(SUBSRC:.cc=.h) 


DICTSRC = catcmd_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

INCDIR = ../../include

AUTOMAKE_OPTIONS = foreign

#INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC  $(shell root-config --cflags) -I$(INCDIR) -I$(TARTSYS)/include -Wall -O2
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF) $(OPTHDR)

lib_LTLIBRARIES = libcatcmd.la
libcatcmd_la_SOURCES = $(SUBSRC) $(DICTSRC)
libcatcmd_la_DEPENDENCIES = header
libcatcmd_la_LDFLAGS = $(LDFLAGS)

$(DICTSRC): $(SUBHDR) catcmd_linkdef.h
	@echo
	@echo Making $(DICTSRC)
	@echo
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
