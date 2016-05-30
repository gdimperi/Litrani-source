#
#  make clean and make install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like litrani, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of litrani.
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
CXXFLAGS      = -O -Wall -fPIC -I$(ROOTDEV)/include 
LD            = g++
LDFLAGS       = -O

LIBNAME       = libLitrani
PROGRAMSO     = $(LIBNAME).$(DllSuf)
PROGRAM       = litrani$(ExeSuf)
SOFLAGS       = -shared -soname=$(PROGRAMSO)
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LIBS         += -L$(ROOTSYS)/lib -lSpectrum -lGui -lHtml -lGuiHtml
LIBS         += -L$(ROOTDEV)/lib -lTwoPad -lSplineFit

#------------------------------------------------------------------------------


HDRS     = TCleanOut.h TLitPhys.h TLitSpectrumCp.h TLitSpectrum.h TLitGlob.h \
           TSComplex.h TPol3.h TPol4.h TMessErr.h \
           T33Matrix.h T33CMatrix.h TSellmeier.h T3CVector.h TEqIndex.h \
           TOpticMaterial.h TSParticle.h TRevetment.h \
           ThinSlice.h T3Vector.h TFace.h TSupplShape.h \
           TSBRIK.h TSTRD1.h TSTRD2.h TSPARA.h TSTRAP.h \
           TSGTRA.h TS8PTS.h TSCYL.h TSTUBE.h TSCONE.h \
           TSNode.h TOtherSide.h TContact.h  TPhotonCradle.h \
           TSpontan.h TTWave.h TPhoton.h TResults.h  \
           TPublication.h TDetector.h TStatSurfD.h TStatVolD.h \
           TStatAPD.h TBeam.h TElecCascade.h TPhotoElecCompton.h
SRCS     = main.$(SrcSuf) TCleanOut.$(SrcSuf) TLitPhys.$(SrcSuf) TLitSpectrumCp.$(SrcSuf) TLitSpectrum.$(SrcSuf) TLitGlob.$(SrcSuf) \
           TSComplex.$(SrcSuf) TPol3.$(SrcSuf) TPol4.$(SrcSuf) TMessErr.$(SrcSuf) \
           TSellmeier.$(SrcSuf) T33Matrix.$(SrcSuf) T33CMatrix.$(SrcSuf) T3CVector.$(SrcSuf) TEqIndex.$(SrcSuf) \
           TOpticMaterial.$(SrcSuf) TSParticle.$(SrcSuf) TRevetment.$(SrcSuf) \
           ThinSlice.$(SrcSuf) T3Vector.$(SrcSuf) TFace.$(SrcSuf) TSupplShape.$(SrcSuf) \
           TSBRIK.$(SrcSuf) TSTRD1.$(SrcSuf) TSTRD2.$(SrcSuf) TSPARA.$(SrcSuf) TSTRAP.$(SrcSuf) \
           TSGTRA.$(SrcSuf) TS8PTS.$(SrcSuf) TSCYL.$(SrcSuf) TSTUBE.$(SrcSuf) TSCONE.$(SrcSuf) \
           TSNode.$(SrcSuf) TOtherSide.$(SrcSuf) TContact.$(SrcSuf) TPhotonCradle.$(SrcSuf) \
           TSpontan.$(SrcSuf) TTWave.$(SrcSuf) TPhoton.$(SrcSuf) TResults.$(SrcSuf) \
           TPublication.$(SrcSuf) TDetector.$(SrcSuf) TStatSurfD.$(SrcSuf) TStatVolD.$(SrcSuf) \
           TStatAPD.$(SrcSuf) TBeam.$(SrcSuf) TElecCascade.$(SrcSuf) TPhotoElecCompton.$(SrcSuf) \
           litraniDict.$(SrcSuf)
OBJS     = TCleanOut.$(ObjSuf) TLitPhys.$(ObjSuf) TLitSpectrumCp.$(ObjSuf) TLitSpectrum.$(ObjSuf) TLitGlob.$(ObjSuf) \
           TSComplex.$(ObjSuf) TPol3.$(ObjSuf) TPol4.$(ObjSuf) TMessErr.$(ObjSuf) \
           TSellmeier.$(ObjSuf) T33Matrix.$(ObjSuf) T33CMatrix.$(ObjSuf) T3CVector.$(ObjSuf) TEqIndex.$(ObjSuf) \
           TOpticMaterial.$(ObjSuf) TSParticle.$(ObjSuf) TRevetment.$(ObjSuf) \
           ThinSlice.$(ObjSuf) T3Vector.$(ObjSuf) TFace.$(ObjSuf) TSupplShape.$(ObjSuf) \
           TSBRIK.$(ObjSuf) TSTRD1.$(ObjSuf) TSTRD2.$(ObjSuf) TSPARA.$(ObjSuf) TSTRAP.$(ObjSuf) \
           TSGTRA.$(ObjSuf) TS8PTS.$(ObjSuf) TSCYL.$(ObjSuf) TSTUBE.$(ObjSuf) TSCONE.$(ObjSuf) \
           TSNode.$(ObjSuf) TOtherSide.$(ObjSuf) TContact.$(ObjSuf) TPhotonCradle.$(ObjSuf) \
           TSpontan.$(ObjSuf) TTWave.$(ObjSuf) TPhoton.$(ObjSuf) TResults.$(ObjSuf) \
           TPublication.$(ObjSuf) TDetector.$(ObjSuf) TStatSurfD.$(ObjSuf) TStatVolD.$(ObjSuf) \
           TStatAPD.$(ObjSuf) TBeam.$(ObjSuf) TElecCascade.$(ObjSuf) TPhotoElecCompton.$(ObjSuf) \
           litraniDict.$(ObjSuf)

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
		@rm -f $(OBJS) main.o *\~ litraniDict.h litraniDict.cpp core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f *.lis *.root
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TCleanOut.h
		@rm -f $(ROOTDEV)/include/TLitSpectrumCp.h
		@rm -f $(ROOTDEV)/include/TLitSpectrum.h
		@rm -f $(ROOTDEV)/include/T33CMatrix.h
		@rm -f $(ROOTDEV)/include/T33Matrix.h
		@rm -f $(ROOTDEV)/include/T3CVector.h
		@rm -f $(ROOTDEV)/include/T3Vector.h
		@rm -f $(ROOTDEV)/include/TBeam.h
		@rm -f $(ROOTDEV)/include/TSComplex.h
		@rm -f $(ROOTDEV)/include/TContact.h
		@rm -f $(ROOTDEV)/include/TDetector.h
		@rm -f $(ROOTDEV)/include/TElecCascade.h
		@rm -f $(ROOTDEV)/include/TEqIndex.h
		@rm -f $(ROOTDEV)/include/TFace.h
		@rm -f $(ROOTDEV)/include/TLitGlob.h
		@rm -f $(ROOTDEV)/include/TMessErr.h
		@rm -f $(ROOTDEV)/include/TOpticMaterial.h
		@rm -f $(ROOTDEV)/include/TOtherSide.h
		@rm -f $(ROOTDEV)/include/TPhoton.h
		@rm -f $(ROOTDEV)/include/TPhotonCradle.h
		@rm -f $(ROOTDEV)/include/TLitPhys.h
		@rm -f $(ROOTDEV)/include/TPol3.h
		@rm -f $(ROOTDEV)/include/TPol4.h
		@rm -f $(ROOTDEV)/include/TPublication.h
		@rm -f $(ROOTDEV)/include/TResults.h
		@rm -f $(ROOTDEV)/include/TRevetment.h
		@rm -f $(ROOTDEV)/include/TS8PTS.h
		@rm -f $(ROOTDEV)/include/TSBRIK.h
		@rm -f $(ROOTDEV)/include/TSCONE.h
		@rm -f $(ROOTDEV)/include/TSCYL.h
		@rm -f $(ROOTDEV)/include/TSGTRA.h
		@rm -f $(ROOTDEV)/include/TSNode.h
		@rm -f $(ROOTDEV)/include/TSPARA.h
		@rm -f $(ROOTDEV)/include/TSParticle.h
		@rm -f $(ROOTDEV)/include/TSTRAP.h
		@rm -f $(ROOTDEV)/include/TSTRD1.h
		@rm -f $(ROOTDEV)/include/TSTRD2.h
		@rm -f $(ROOTDEV)/include/TSTUBE.h
		@rm -f $(ROOTDEV)/include/TSellmeier.h
		@rm -f $(ROOTDEV)/include/TSpontan.h
		@rm -f $(ROOTDEV)/include/TStatAPD.h
		@rm -f $(ROOTDEV)/include/TStatSurfD.h
		@rm -f $(ROOTDEV)/include/TStatVolD.h
		@rm -f $(ROOTDEV)/include/TSupplShape.h
		@rm -f $(ROOTDEV)/include/TTWave.h
		@rm -f $(ROOTDEV)/include/ThinSlice.h
		@rm -f $(ROOTDEV)/include/TPhotoElecCompton.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TCleanOut.h
		@rm -f $(ROOTDEV)/include/TLitSpectrumCp.h
		@rm -f $(ROOTDEV)/include/TLitSpectrum.h
		@rm -f $(ROOTDEV)/include/T33CMatrix.h
		@rm -f $(ROOTDEV)/include/T33Matrix.h
		@rm -f $(ROOTDEV)/include/T3CVector.h
		@rm -f $(ROOTDEV)/include/T3Vector.h
		@rm -f $(ROOTDEV)/include/TBeam.h
		@rm -f $(ROOTDEV)/include/TSComplex.h
		@rm -f $(ROOTDEV)/include/TContact.h
		@rm -f $(ROOTDEV)/include/TDetector.h
		@rm -f $(ROOTDEV)/include/TElecCascade.h
		@rm -f $(ROOTDEV)/include/TEqIndex.h
		@rm -f $(ROOTDEV)/include/TFace.h
		@rm -f $(ROOTDEV)/include/TLitGlob.h
		@rm -f $(ROOTDEV)/include/TMessErr.h
		@rm -f $(ROOTDEV)/include/TOpticMaterial.h
		@rm -f $(ROOTDEV)/include/TOtherSide.h
		@rm -f $(ROOTDEV)/include/TPhoton.h
		@rm -f $(ROOTDEV)/include/TPhotonCradle.h
		@rm -f $(ROOTDEV)/include/TLitPhys.h
		@rm -f $(ROOTDEV)/include/TPol3.h
		@rm -f $(ROOTDEV)/include/TPol4.h
		@rm -f $(ROOTDEV)/include/TPublication.h
		@rm -f $(ROOTDEV)/include/TResults.h
		@rm -f $(ROOTDEV)/include/TRevetment.h
		@rm -f $(ROOTDEV)/include/TS8PTS.h
		@rm -f $(ROOTDEV)/include/TSBRIK.h
		@rm -f $(ROOTDEV)/include/TSCONE.h
		@rm -f $(ROOTDEV)/include/TSCYL.h
		@rm -f $(ROOTDEV)/include/TSGTRA.h
		@rm -f $(ROOTDEV)/include/TSNode.h
		@rm -f $(ROOTDEV)/include/TSPARA.h
		@rm -f $(ROOTDEV)/include/TSParticle.h
		@rm -f $(ROOTDEV)/include/TSTRAP.h
		@rm -f $(ROOTDEV)/include/TSTRD1.h
		@rm -f $(ROOTDEV)/include/TSTRD2.h
		@rm -f $(ROOTDEV)/include/TSTUBE.h
		@rm -f $(ROOTDEV)/include/TSellmeier.h
		@rm -f $(ROOTDEV)/include/TSpontan.h
		@rm -f $(ROOTDEV)/include/TStatAPD.h
		@rm -f $(ROOTDEV)/include/TStatSurfD.h
		@rm -f $(ROOTDEV)/include/TStatVolD.h
		@rm -f $(ROOTDEV)/include/TSupplShape.h
		@rm -f $(ROOTDEV)/include/TTWave.h
		@rm -f $(ROOTDEV)/include/ThinSlice.h
		@rm -f $(ROOTDEV)/include/TPhotoElecCompton.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(LIBNAME).$(DllSuf) $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@cp TCleanOut.h $(ROOTDEV)/include/TCleanOut.h
		@cp TLitSpectrumCp.h $(ROOTDEV)/include/TLitSpectrumCp.h
		@cp TLitSpectrum.h $(ROOTDEV)/include/TLitSpectrum.h
		@cp T33CMatrix.h $(ROOTDEV)/include/T33CMatrix.h
		@cp T33Matrix.h $(ROOTDEV)/include/T33Matrix.h
		@cp T3CVector.h $(ROOTDEV)/include/T3CVector.h
		@cp T3Vector.h $(ROOTDEV)/include/T3Vector.h
		@cp TBeam.h $(ROOTDEV)/include/TBeam.h
		@cp TSComplex.h $(ROOTDEV)/include/TSComplex.h
		@cp TContact.h $(ROOTDEV)/include/TContact.h
		@cp TDetector.h $(ROOTDEV)/include/TDetector.h
		@cp TElecCascade.h $(ROOTDEV)/include/TElecCascade.h
		@cp TEqIndex.h $(ROOTDEV)/include/TEqIndex.h
		@cp TFace.h $(ROOTDEV)/include/TFace.h
		@cp TLitGlob.h $(ROOTDEV)/include/TLitGlob.h
		@cp TMessErr.h $(ROOTDEV)/include/TMessErr.h
		@cp TOpticMaterial.h $(ROOTDEV)/include/TOpticMaterial.h
		@cp TOtherSide.h $(ROOTDEV)/include/TOtherSide.h
		@cp TPhoton.h $(ROOTDEV)/include/TPhoton.h
		@cp TPhotonCradle.h $(ROOTDEV)/include/TPhotonCradle.h
		@cp TLitPhys.h $(ROOTDEV)/include/TLitPhys.h
		@cp TPol3.h $(ROOTDEV)/include/TPol3.h
		@cp TPol4.h $(ROOTDEV)/include/TPol4.h
		@cp TPublication.h $(ROOTDEV)/include/TPublication.h
		@cp TResults.h $(ROOTDEV)/include/TResults.h
		@cp TRevetment.h $(ROOTDEV)/include/TRevetment.h
		@cp TS8PTS.h $(ROOTDEV)/include/TS8PTS.h
		@cp TSBRIK.h $(ROOTDEV)/include/TSBRIK.h
		@cp TSCONE.h $(ROOTDEV)/include/TSCONE.h
		@cp TSCYL.h $(ROOTDEV)/include/TSCYL.h
		@cp TSGTRA.h $(ROOTDEV)/include/TSGTRA.h
		@cp TSNode.h $(ROOTDEV)/include/TSNode.h
		@cp TSPARA.h $(ROOTDEV)/include/TSPARA.h
		@cp TSParticle.h $(ROOTDEV)/include/TSParticle.h
		@cp TSTRAP.h $(ROOTDEV)/include/TSTRAP.h
		@cp TSTRD1.h $(ROOTDEV)/include/TSTRD1.h
		@cp TSTRD2.h $(ROOTDEV)/include/TSTRD2.h
		@cp TSTUBE.h $(ROOTDEV)/include/TSTUBE.h
		@cp TSellmeier.h $(ROOTDEV)/include/TSellmeier.h
		@cp TSpontan.h $(ROOTDEV)/include/TSpontan.h
		@cp TStatAPD.h $(ROOTDEV)/include/TStatAPD.h
		@cp TStatSurfD.h $(ROOTDEV)/include/TStatSurfD.h
		@cp TStatVolD.h $(ROOTDEV)/include/TStatVolD.h
		@cp TSupplShape.h $(ROOTDEV)/include/TSupplShape.h
		@cp TTWave.h $(ROOTDEV)/include/TTWave.h
		@cp ThinSlice.h $(ROOTDEV)/include/ThinSlice.h
		@cp TPhotoElecCompton.h $(ROOTDEV)/include/TPhotoElecCompton.h
		@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###
TCleanOut.$(ObjSuf):          TCleanOut.h
TLitSpectrumCp.$(ObjSuf):     TLitSpectrumCp.h
TLitSpectrum.$(ObjSuf):       TLitSpectrumCp.h TLitSpectrum.h
TLitPhys.$(ObjSuf):           TCleanOut.h TLitPhys.h
TSComplex.$(ObjSuf):          TSComplex.h
TPol3.$(ObjSuf):              TSComplex.h TLitPhys.h TPol3.h
TPol4.$(ObjSuf):              TSComplex.h TPol3.h TPol4.h
TMessErr.$(ObjSuf):           TSComplex.h T3Vector.h TMessErr.h
TSellmeier.$(ObjSuf):         TMessErr.h TSellmeier.h
T33Matrix.$(ObjSuf):          TMessErr.h T33Matrix.h
T33CMatrix.$(ObjSuf):         TSComplex.h TMessErr.h T33Matrix.h T33CMatrix.h
T3Vector.$(ObjSuf):           TMessErr.h T33Matrix.h T3Vector.h
T3CVector.$(ObjSuf):          TSComplex.h TMessErr.h T33CMatrix.h T3Vector.h T3CVector.h
TEqIndex.$(ObjSuf):           TSComplex.h TPol4.h TMessErr.h TLitGlob.h TEqIndex.h
TOpticMaterial.$(ObjSuf):     TMessErr.h TLitPhys.h TLitSpectrum.h \
                              TSellmeier.h TLitGlob.h TOpticMaterial.h
TSParticle.$(ObjSuf):         TLitPhys.h TMessErr.h TLitGlob.h TSParticle.h
TRevetment.$(ObjSuf):         TSComplex.h TMessErr.h TOpticMaterial.h \
                              TLitGlob.h TRevetment.h
ThinSlice.$(ObjSuf):          TMessErr.h TLitGlob.h ThinSlice.h
TMessErr.$(ObjSuf):           TSComplex.h T3Vector.h TMessErr.h
TFace.$(ObjSuf):              TMessErr.h T3Vector.h TRevetment.h TLitGlob.h \
                              TContact.h TSupplShape.h TFace.h
TDetector.$(ObjSuf):          TMessErr.h TSupplShape.h TFace.h TLitGlob.h TDetector.h
TStatSurfD.$(ObjSuf):         TLitPhys.h TStatSurfD.h
TStatVolD.$(ObjSuf):          TLitPhys.h TStatVolD.h
TStatAPD.$(ObjSuf):           TLitPhys.h TStatAPD.h
TSBRIK.$(ObjSuf):             T3Vector.h TFace.h TSupplShape.h TSBRIK.h
TSTRD1.$(ObjSuf):             T3Vector.h TFace.h TSupplShape.h TSTRD1.h
TSTRD2.$(ObjSuf):             T3Vector.h TFace.h TSupplShape.h TSTRD2.h
TSPARA.$(ObjSuf):             T3Vector.h TMessErr.h TFace.h TSupplShape.h TSPARA.h
TSTRAP.$(ObjSuf):             T3Vector.h TMessErr.h TFace.h TSupplShape.h TSTRAP.h
TSGTRA.$(ObjSuf):             T3Vector.h TMessErr.h TFace.h TSupplShape.h TSGTRA.h
TS8PTS.$(ObjSuf):             T3Vector.h TMessErr.h TFace.h TSupplShape.h TS8PTS.h
TSCYL.$(ObjSuf):              T3Vector.h TFace.h TSupplShape.h TSCYL.h
TSTUBE.$(ObjSuf):             T3Vector.h TFace.h TSupplShape.h TSTUBE.h
TSCONE.$(ObjSuf):             T3Vector.h TMessErr.h TFace.h TSupplShape.h TSCONE.h
TSNode.$(ObjSuf):             T3Vector.h TMessErr.h TLitGlob.h TSupplShape.h TSNode.h
TOtherSide.$(ObjSuf):         TFace.h TContact.h TOtherSide.h
TContact.$(ObjSuf):           TMessErr.h TOtherSide.h TFace.h TSupplShape.h TLitGlob.h ThinSlice.h \
                              TContact.h
TPhotonCradle.$(ObjSuf):      TMessErr.h TOpticMaterial.h TSupplShape.h \
                              TSNode.h TPhotonCradle.h
TSpontan.$(ObjSuf):           TMessErr.h TSupplShape.h TPhoton.h TEqIndex.h \
                              TLitGlob.h T3Vector.h \
                              TLitPhys.h TOpticMaterial.h TResults.h TPublication.h \
			      TSpontan.h
TTWave.$(ObjSuf):             TSComplex.h T3Vector.h TMessErr.h TTWave.h
TLitGlob.$(ObjSuf):           TMessErr.h TLitPhys.h TEqIndex.h TOpticMaterial.h \
                              TRevetment.h ThinSlice.h TSupplShape.h TContact.h \
                              TDetector.h TResults.h TPublication.h TSParticle.h \
                              TBeam.h TSpontan.h TLitGlob.h
TSupplShape.$(ObjSuf):        T33CMatrix.h T3Vector.h TLitGlob.h \
                              TMessErr.h TFace.h TLitPhys.h TOpticMaterial.h TRevetment.h TDetector.h \
                              TSBRIK.h TSTRD1.h TSTRD2.h TSPARA.h TSTRAP.h \
                              TS8PTS.h TSGTRA.h TSCYL.h TSTUBE.h TSCONE.h \
                              TSupplShape.h
TPhoton.$(ObjSuf):            TSComplex.h TMessErr.h T3Vector.h TFace.h TLitPhys.h  TLitGlob.h \
                              TOpticMaterial.h TRevetment.h ThinSlice.h TSupplShape.h \
			      TContact.h TStatSurfD.h TStatVolD.h TStatAPD.h \
			      TResults.h TTWave.h TEqIndex.h T33Matrix.h T33CMatrix.h \
			      T3CVector.h TPhoton.h
TResults.$(ObjSuf):           TLitGlob.h TMessErr.h TOpticMaterial.h TDetector.h \
                              TStatSurfD.h TStatVolD.h TStatAPD.h TSupplShape.h \
			      TFace.h TResults.h
TPublication.$(ObjSuf):       TLitGlob.h TResults.h TStatSurfD.h TStatVolD.h \
                              TStatAPD.h TMessErr.h TPublication.h
TBeam.$(ObjSuf):              TLitGlob.h T3Vector.h TLitPhys.h TMessErr.h TOpticMaterial.h \
                              TSupplShape.h TSNode.h TResults.h TPublication.h TEqIndex.h TPhoton.h \
		              TSParticle.h TPhotonCradle.h TBeam.h
TElecCascade.$(ObjSuf):       TLitGlob.h TMessErr.h T3Vector.h TOpticMaterial.h TSupplShape.h \
                              TSNode.h TEqIndex.h TResults.h TPublication.h TPhotonCradle.h \
		              TPhoton.h TElecCascade.h
TPhotoElecCompton.$(ObjSuf):  TMessErr.h TLitGlob.h TOpticMaterial.h TSupplShape.h TSNode.h \
                              TEqIndex.h TResults.h TPublication.h TPhotonCradle.h TPhoton.h \
                              TPhotoElecCompton.h


.SUFFIXES: .$(SrcSuf)

###
litraniDict.$(SrcSuf): $(HDRS)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f litraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

