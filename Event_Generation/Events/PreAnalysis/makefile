Target:=LamWZPreAnalysis.x
Mainfile:=LamWZPreAnalysis.cpp
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


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)



# CubaCFLAGS := -I$(CubaPATH)
# CubaLIBS := -L$(CubaPATH)/ -lcuba -lm
# CubaSPECIAL := -Xlinker -rpath $(CubaPATH)


#INCDIRS	+= $(EXTRAHDR)
INCLUDES      = $(ROOTCFLAGS) $(OOLibCFLAGS) $(addprefix -I, $(INCDIRS) ) 
#$(CubaCFLAGS) 
CXX           = $(shell root-config --cxx)
LD            = $(shell root-config --ld)
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
LIBS         += $(ROOTLIBS) $(SYSLIBS) $(ExRootLIBS) $(DELPHESLIBS) $(OOLibLIBS) -lpthread 
#$(CubaLIBS) 
GLIBS        += $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ) 
# $(ObjDir)/EachEventDef.$(ObjSuf) $(ObjDir)/TreeReaderDef.$(ObjSuf)

#------------------------------------------------------------------------------
all: $(Target)

$(Target):	$(OBJS) $(Mainfile)
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(Mainfile) -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) $(ExRootSPECIAL) $(DELPHESSPECIAL) $(OOLibSPECIAL) $(CubaSPECIAL)
		@echo "--->$@ done"

# GMWAPreAnalysisPlain:  $(OBJS) GMWAPreAnalysisPlain.cpp
# 		@$(LD) $(LDFLAGS) $(CXXFLAGS) GMWAPreAnalysisPlain.cpp -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) $(ExRootSPECIAL) $(DELPHESSPECIAL) $(OOLibSPECIAL) $(CubaSPECIAL)
# 		@echo "--->$@ done"

$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<


# $(SrcDir)/EachEventDef.$(SrcSuf):$(IncDir)/EachEvent.h $(IncDir)/EachEventLinkDef.h
# 	@echo "Generating dictionary $@..."
# 	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^

# $(SrcDir)/TreeReaderDef.$(SrcSuf):$(IncDir)/TreeReader.h $(IncDir)/TreeReaderLinkDef.h
# 	@echo "Generating dictionary $@..."
# 	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^
#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) $(Target)
#$(SrcDir)/EachEventDef.$(SrcSuf) $(SrcDir)/EachEventDef.$(IncSuf) $(SrcDir)/TreeReaderDef.$(SrcSuf) $(SrcDir)/TreeReaderDef.$(IncSuf)
	@echo "--->Clean Done"
