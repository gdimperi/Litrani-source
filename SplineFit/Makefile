#
#  make clean and make install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like SplineFit, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of SplineFit.
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

CXX           = g++
#CXXFLAGS      = -g -m32 -Wall -fPIC -I$(ROOTDEV)/include 
CXXFLAGS      = -O -m32 -Wall -fPIC -I$(ROOTDEV)/include
LD            = g++
#LDFLAGS       = -g -m32
LDFLAGS       = -O -m32

LIBNAME       = libSplineFit
PROGRAMSO     = $(LIBNAME).$(DllSuf)
PROGRAM       = SplineFit$(ExeSuf)
SOFLAGS       = -shared -soname=$(PROGRAMSO)
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LIBS         += -L$(ROOTSYS)/lib -lGui -lHtml -lGuiHtml
LIBS         += -L$(ROOTDEV)/lib -lTwoPad

#------------------------------------------------------------------------------



HDRS          = TPoly3.h TBandedLE.h TZigZag.h TSplineFit.h
SRCS          = main.$(SrcSuf) TPoly3.$(SrcSuf) TBandedLE.$(SrcSuf) \
                TZigZag.$(SrcSuf) TSplineFit.$(SrcSuf) SplineFitDict.$(SrcSuf)
OBJS          = TPoly3.$(ObjSuf) TBandedLE.$(ObjSuf) TZigZag.$(ObjSuf) \
                TSplineFit.$(ObjSuf) SplineFitDict.$(ObjSuf)


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
		@rm -f $(OBJS) main.$(ObjSuf) *\~ SplineFitDict.h SplineFitDict.$(SrcSuf) core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f *.lis *.root
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(PROGRAMSO)
		@rm -f $(ROOTDEV)/include/TPoly3.h
		@rm -f $(ROOTDEV)/include/TBandedLE.h
		@rm -f $(ROOTDEV)/include/TZigZag.h
		@rm -f $(ROOTDEV)/include/TSplineFit.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TPoly3.h
		@rm -f $(ROOTDEV)/include/TBandedLE.h
		@rm -f $(ROOTDEV)/include/TZigZag.h
		@rm -f $(ROOTDEV)/include/TSplineFit.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(PROGRAMSO) $(ROOTDEV)/lib/$(PROGRAMSO)
		@cp TPoly3.h $(ROOTDEV)/include/TPoly3.h
		@cp TBandedLE.h $(ROOTDEV)/include/TBandedLE.h
		@cp TZigZag.h $(ROOTDEV)/include/TZigZag.h
		@cp TSplineFit.h $(ROOTDEV)/include/TSplineFit.h
		@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###

TPoly3.$(ObjSuf):         TPoly3.h
TBandedLE.$(ObjSuf):      TBandedLE.h
TZigZag.$(ObjSuf):        TZigZag.h
TSplineFit.$(ObjSuf):     TPoly3.h TBandedLE.h TZigZag.h TSplineFit.h


.SUFFIXES: .$(SrcSuf)

###
SplineFitDict.$(SrcSuf): $(HDRS)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f SplineFitDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

