#
#  make clean and make install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like VisuLitrani, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of VisuLitrani.
#  If you have write access to $(ROOTSYS), you can choose ROOTDEV=ROOTSYS, but this mixing
#of your code with the code of ROOT is to my mind inelegant and the choice of a separate
#ROOTDEV is surely better.
# $(ROOTDEV)/bin  has to be added to PATH
# $(ROOTDEV)/lib  has to be added to LD_LIBRARY_PATH
#
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

# Linux with egcs
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC -I$(ROOTDEV)/include 
LD            = g++
LDFLAGS       = -O
PROGRAMSO = $(LIBNAME).$(DllSuf)
SOFLAGS       = -shared -DT_SONAME=$(PROGRAMSO) 

LIBNAME       = libVisuLitrani
PROGRAMLIB    = $(LIBNAME).lib
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LIBS         += -L$(ROOTDEV)/lib  -lTwoPad  -lLitrani -lSplineFit  
LIBS         += -L$(ROOTLIBS) -lSpectrum -lGui -lHtml -lGuiHtml
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------


HDRS          = TVisuLit.h
SRCS          = main.$(SrcSuf) TVisuLit.$(SrcSuf) VisuLitraniDict.$(SrcSuf)
OBJS          = TVisuLit.$(ObjSuf) VisuLitraniDict.$(ObjSuf)


PROGRAM   = VisuLitrani$(ExeSuf)

all:        $(PROGRAMSO) $(PROGRAM)

$(PROGRAMSO): $(OBJS)
		@echo "Creating library $(PROGRAMSO) ..."
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@
		@echo "$(PROGRAMSO) done"

$(PROGRAM): main.$(ObjSuf) $(OBJS)
		@echo "Linking $(PROGRAM) ..."
		$(LD) $(LDFLAGS) main.$(ObjSuf) $(OBJS) $(LIBS) $(OutPutOpt)$(PROGRAM)
		@echo "$(PROGRAM) done"

clean:
		@rm -f $(OBJS) main.o *\~ VisuLitraniDict.h VisuLitraniDict.cpp core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TVisuLit.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TVisuLit.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(LIBNAME).$(DllSuf) $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@cp TVisuLit.h $(ROOTDEV)/include/TVisuLit.h
		@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###
TVisuLit.$(ObjSuf):   TVisuLit.h


.SUFFIXES: .$(SrcSuf)

###
VisuLitraniDict.$(SrcSuf): $(HDRS)
	@echo "Deleting Dictionary..."
	@rm -f VisuLitraniDict.h VisuLitraniDict.$(SrcSuf) VisuLitraniDict.$(ObjSuf)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f VisuLitraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

