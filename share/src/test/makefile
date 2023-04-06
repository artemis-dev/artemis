VPATH = cat coin command cont converter decoder mwdc plastic ppac scaler

OBJ =
OBJ += constant_test.o
OBJ += TTimingData_test.o
OBJ += TChargeData_test.o
OBJ += TTimingChargeData_test.o
OBJ += TTimeDifference_test.o
OBJ += TPPACData_test.o
OBJ += TMWDCHitData_test.o
OBJ += TAffineConverter_test.o

TARGET = do_test

ARTEMIS_HOME ?= /opt/local

INCLUDE = -I. -I.. $(addprefix -I../, $(VPATH))

CXXFLAGS = -Wall $(INCLUDE) -I$(ARTEMIS_HOME)/include -fPIC `root-config --cflags` 
LDFLAGS = -L$(ARTEMIS_HOME)/lib -L.. -luser -lcatloop -lcatcore -lcatmc -larthist -lartparam -lartridf -lMinuit `root-config --libs`

GTESTOBJ = gtest/gtest-all.o gtest/gtest_main.o

all: $(TARGET)
.PHONY: all test clean

$(TARGET): $(OBJ) $(GTESTOBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(GTESTOBJ):
	$(CXX) -I./ -c gtest/gtest-all.cc gtest/gtest_main.cc
	@mv gtest*.o gtest

-include $(OBJ:.o=.d)

%.d: %.cc
	g++ -MM $(CXXFLAGS) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.d *.o $(TARGET) $(GTESTOBJ)
