Target1:=LamWZPlot.x
Mainfile1:=LamWZPlot.cpp
#------------------------------------------------------------------------------
maindir := $(shell pwd)
SrcSuf := cpp
IncSuf := h
ObjSuf := o
DepSuf := d
DllSuf := so

SrcDir := src
IncDir := include
ObjDir := obj
DepDir := $(ObjDir)

INCDIRS = $(shell pwd)/include $(shell pwd)
EXTRAHDR = 
#------------------------------------------------------------------------------
# Architecture to compile for
ARCH          = linux
#ARCH          = solarisCC5

#CubaPATH := $(maindir)/../Cuba-4.2

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMLP -lMinuit -lTreePlayer -lTMVA -lXMLIO  -lMLP -lm -lRooFit -lRooFitCore -lRooStats
ROOTGLIBS    := $(shell root-config --glibs)


#INCDIRS	+= $(EXTRAHDR)
INCLUDES      = $(ROOTCFLAGS) $(addprefix -I, $(INCDIRS) ) 
#$(OOLibCFLAGS) $(CubaCFLAGS) 
CXX           = g++
LD            = g++
SOFLAGS       = -shared

ifdef DEBUG
  CXXFLAGS    = -g -O0 
  LDFLAGS     = -g -O0
  DEFINES     =
else
  CXXFLAGS    = -O3
  LDFLAGS     = -O3
  DEFINES     = 
#  DEFINES     = -DNDEBUG
endif

DEFINES      += -DLINUXVERS -DHAS_SSTREAM
ifdef VERBOSE
DEFINES      += -DVERBOSE
endif
ifdef TESTCODE
DEFINES      += -DTESTCODE
endif

CXXFLAGS     += $(DEFINES) $(INCLUDES)
LIBS         += $(ROOTLIBS) $(SYSLIBS) 
#$(ExRootLIBS) $(DELPHESLIBS) $(OOLibLIBS) $(CubaLIBS) -lpthread
GLIBS        += $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ) 
# $(ObjDir)/EachEventDef.$(ObjSuf) $(ObjDir)/TreeReaderDef.$(ObjSuf)

all:$(Target1)
#------------------------------------------------------------------------------

$(Target1):	$(OBJS) $(Mainfile1)
		@$(LD) -g $(LDFLAGS) $(CXXFLAGS) $(Mainfile1) -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) 
		#$(ExRootSPECIAL) $(DELPHESSPECIAL) $(OOLibSPECIAL) $(CubaSPECIAL)
		@echo "--->$@ done"

%.x:%.cpp $(OBJS) 
		@$(LD) -g $(LDFLAGS) $(CXXFLAGS) $< -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) 
		#$(ExRootSPECIAL) $(DELPHESSPECIAL) $(OOLibSPECIAL) $(CubaSPECIAL)
		@echo "--->$@ done"


$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@$(CXX) -g $(CXXFLAGS) -o $@ -c $<


# $(SrcDir)/EachEventDef.$(SrcSuf):$(IncDir)/EachEvent.h $(IncDir)/EachEventLinkDef.h
# 	@echo "Generating dictionary $@..."
# 	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^

# $(SrcDir)/TreeReaderDef.$(SrcSuf):$(IncDir)/TreeReader.h $(IncDir)/TreeReaderLinkDef.h
# 	@echo "Generating dictionary $@..."
# 	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^
#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) *.x
#$(SrcDir)/EachEventDef.$(SrcSuf) $(SrcDir)/EachEventDef.$(IncSuf) $(SrcDir)/TreeReaderDef.$(SrcSuf) $(SrcDir)/TreeReaderDef.$(IncSuf)
	@echo "--->Clean Done"
