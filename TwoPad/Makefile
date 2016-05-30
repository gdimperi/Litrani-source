#
#  make clean and make install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like Litrani, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of Litrani.
#  If you have write access to $(ROOTSYS), you can choose ROOTDEV=ROOTSYS, but this mixing
#of your code with the code of ROOT is to my mind inelegant and the choice of a separate
#ROOTDEV is surely better.
# $(ROOTDEV)/bin  has to be added to PATH
# $(ROOTDEV)/lib  has to be added to LD_LIBRARY_PATH
#
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

# Linux with egcs
CXX           = g++
#CXXFLAGS      = -g -m32 -Wall -fPIC
CXXFLAGS      = -O -m32 -Wall -fPIC
LD            = g++
#LDFLAGS       = -g -m32
LDFLAGS       = -O -m32
SOFLAGS       = -shared

LIBNAME       = libTwoPad
PROGRAMLIB    = $(LIBNAME).lib
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LIBS         += -L$(ROOTSYS)/lib -lGui -lHtml -lGuiHtml
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

HDRS          = TGRootIDE.h TAuthor.h TReference.h TwoPadDisplay.h
SRCS          = main.$(SrcSuf) TGRootIDE.$(SrcSuf) TAuthor.$(SrcSuf) TReference.$(SrcSuf) \
                TwoPadDisplay.$(SrcSuf) TwoPadDict.$(SrcSuf)
OBJS          = TGRootIDE.$(ObjSuf) TReference.$(ObjSuf) TAuthor.$(ObjSuf) TwoPadDisplay.$(ObjSuf) \
                TwoPadDict.$(ObjSuf)

PROGRAMSO = $(LIBNAME).$(DllSuf)
PROGRAM   = TwoPad$(ExeSuf)

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
		@rm -f $(OBJS) main.o *\~ TwoPadDict.h TwoPadDict.$(SrcSuf) core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f *.lis *.root
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TGRootIDE.h
		@rm -f $(ROOTDEV)/include/TAuthor.h
		@rm -f $(ROOTDEV)/include/TReference.h
		@rm -f $(ROOTDEV)/include/TwoPadDisplay.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TGRootIDE.h
		@rm -f $(ROOTDEV)/include/TAuthor.h
		@rm -f $(ROOTDEV)/include/TReference.h
		@rm -f $(ROOTDEV)/include/TwoPadDisplay.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(LIBNAME).$(DllSuf) $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@cp TGRootIDE.h $(ROOTDEV)/include/TGRootIDE.h
		@cp TAuthor.h $(ROOTDEV)/include/TAuthor.h
		@cp TReference.h $(ROOTDEV)/include/TReference.h
		@cp TwoPadDisplay.h $(ROOTDEV)/include/TwoPadDisplay.h
		@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###

TGRootIDE.$(ObjSuf):     TGRootIDE.h
TAuthor.$(ObjSuf):       TAuthor.h
TReference.$(ObjSuf):    TAuthor.h TReference.h
TwoPadDisplay.$(ObjSuf): TGRootIDE.h TReference.h TwoPadDisplay.h

.SUFFIXES: .$(SrcSuf)

TwoPadDict.$(SrcSuf): $(HDRS)
	@echo "Generating dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f TwoPadDict.$(SrcSuf) -c $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

