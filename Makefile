# ===============================
# Makefile for W+Jets Analysis
# ===============================

# Determine if working at CERN on lxplus machine or at IIHE on m machines
HOST = $(shell hostname -f | grep cern)
ifneq ($(HOST),)
    HOST = "lxplus"
else
    HOST = "iihe"
endif

# define LHAPDF library Path accroding to the host
ifeq ($(HOST), "lxplus")
  ifeq ($(LHAPDF_DATA_PATH),)
    LHAPDFPATH = /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/
  else
    LHAPDFPATH = $(LHAPDF_DATA_PATH)/../../
  endif
else
  ifeq ($(HOST), "iihe")
    LHAPDFPATH = /user/aleonard/LHAPDF/
  endif
endif

# get Root compiler flags
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)

LIB = LHAPDF
LIBLIST = $(addprefix -l, $(LIB))

LIBDIR = RooUnfold $(LHAPDFPATH)lib
LIBDIRLIST = $(addprefix -L, $(LIBDIR))

RTL = -Wl,-rpath,
LIBDIRLISTRTL = $(addprefix $(RTL), $(LIBDIR))

SRCDIR = Sources/
INCDIR = -IIncludes/ -IRooUnfold/src -I$(LHAPDFPATH)include

CXX           = g++ -std=c++0x
CXXFLAGS      = -g -fPIC -O0
CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += -MD -Wall
#CPPFLAGS      += $(INCDIR)

# list of all source files
SOURCES = \
ArgParser.cc variablesOfInterestZJets.cc getFilesAndHistogramsZJets.cc \
ConfigVJets.cc functions.cc UnfoldingZJets.cc UnfoldingZJetsRatios.cc \
PlotSettings.cc CombineUnfoldedRun2.cc 

SRCLIST = $(addprefix $(SRCDIR), $(SOURCES))
OBJLIST = $(SRCLIST:.cc=.o)
DEPS    = $(SRCLIST:.cc=.d)

.PHONY: all clean

all: runUnfoldingZJets \
runUnfoldingZJetsRatios \
runCombineUnfoldedRun2

# ===========================================================================
%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCDIR)

#I'm not sure why this generic rule does not work...
#run%: run%.o $(OBJLIST) RooUnfold/libRooUnfold.so
#	$(CXX) -o $@ $(LIBDIRLIST) $(LIBDIRLISTRTL) $^ $(ROOTGLIBS) $(LIBLIST)

runUnfoldingZJets: runUnfoldingZJets.o $(OBJLIST) RooUnfold/libRooUnfold.so
	$(CXX) -o $@ $(LIBDIRLIST) $(LIBDIRLISTRTL) $^ $(ROOTGLIBS) $(LIBLIST)

runUnfoldingZJetsRatios: runUnfoldingZJetsRatios.o $(OBJLIST) RooUnfold/libRooUnfold.so
	$(CXX) -o $@ $(LIBDIRLIST) $(LIBDIRLISTRTL) $^ $(ROOTGLIBS) $(LIBLIST)

runCombineUnfoldedRun2: runCombineUnfoldedRun2.o $(OBJLIST) RooUnfold/libRooUnfold.so 
	$(CXX) -o $@ $(LIBDIRLIST) $(LIBDIRLISTRTL) $^ $(ROOTGLIBS) $(LIBLIST)

RooUnfold/libRooUnfold.so:
	$(MAKE) -C RooUnfold

# ===========================================================================
clean:
	rm -f *.o Sources/*.o
	$(MAKE) -C RooUnfold clean

clean-dep:
	rm -f *.d Sources/*.d
	$(MAKE) -C RooUnfold clean

echo_%:
	@echo $* = $($*)

-include $(DEPS)
