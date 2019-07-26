SYSTEMC_AMS=$$SCAMS_HOME
SYSTEMC=$$SYSTEMC_HOME

PLATFORM= $(shell uname -s)
ARCH=$(shell getconf LONG_BIT)

ifeq ($(PLATFORM),Darwin)
SYSC_LIBDIR := $(SYSTEMC)/lib-macosx
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-macosx
endif
ifeq ($(PLATFORM),Linux)
SYSC_LIBDIR := $(SYSTEMC)/lib-linux
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-linux
endif

ifeq ($(ARCH),64)
SYSC_LIBDIR := $(SYSC_LIBDIR)64
SCAMS_LIBDIR := $(SCAMS_LIBDIR)64
endif

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I$(SYSTEMC_AMS)/include/

LIBS = $(SCAMS_LIBDIR)/libsystemc-ams.a \
	   $(SYSC_LIBDIR)/libsystemc.a \
	   -lpthread
CXX=g++
CFLAGS= -c
LD=g++

EXECUTABLE = bin/main.x

all: $(EXECUTABLE)

install: all

$(EXECUTABLE): obj/main.o obj/valve_tdf.o obj/watertank_lsf.o  obj/controller_tdf.o obj/System.o obj/global.o
	@echo Building $@
	@$(LD) $(LDFLAGS) $(INCDIR) -o $@ $^ $(LIBS)


################################# Main ########################################
obj/main.o: src/main.cc
	@echo Building $@
	@$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<


############################## TDF Modules ####################################
obj/valve_tdf.o: src/valve_tdf.cc include/valve_tdf.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<

obj/controller_tdf.o: src/controller_tdf.cc include/controller_tdf.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<


############################## LSF Modules ####################################
obj/watertank_lsf.o: src/watertank_lsf.cc include/watertank_lsf.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<


################################# Others ######################################
obj/System.o: src/System.cc include/System.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<

obj/global.o: src/global.cc include/global.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<

clean:
	@echo "Cleaning..."
	@rm -rf obj/*.o
	@rm -rf *.x
	@rm -rf *.vcd
	@rm -rf $(EXECUTABLE)
	@echo "Done!"

.PHONY: all clean
