#
#  nmake clean and nmake install of this makefile imply that you define the system variable
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
ObjSuf        = obj
SrcSuf        = cpp
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:

# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
CXXFLAGS      = $(cvarsdll) -EHsc  -nologo -GR -DWIN32 \
                -D_WINDOWS -I$(ROOTDEV)/include -I$(ROOTSYS)/include -FIw32pragma.h 
LD            = $(link)

CXXOPT       = -O2 -MD
#CXXOPT        = -Z7 -MDd
LDOPT         = -opt:ref
#LDOPT         = -debug

EXTRAFLAGS    = -D_CRT_SECURE_NO_DEPRECATE
LDFLAGS       = $(LDOPT) $(conlflags) -nologo -include:_G__cpp_setupG__Hist \
                -include:_G__cpp_setupG__Graf -include:_G__cpp_setupG__G3D \
                -include:_G__cpp_setupG__GPad -include:_G__cpp_setupG__Tree \
                -include:_G__cpp_setupG__Rint -include:_G__cpp_setupG__PostScript \
                -include:_G__cpp_setupG__Matrix -include:_G__cpp_setupG__Physics
SOFLAGS       = $(dlllflags:-pdb:none=)
ROOTLIBS      = $(ROOTSYS)/lib/libCore.lib \
                $(ROOTSYS)/lib/libCint.lib $(ROOTSYS)/lib/libHist.lib \
                $(ROOTSYS)/lib/libGraf.lib $(ROOTSYS)/lib/libGraf3d.lib \
                $(ROOTSYS)/lib/libGpad.lib $(ROOTSYS)/lib/libTree.lib \
                $(ROOTSYS)/lib/libRint.lib $(ROOTSYS)/lib/libPostscript.lib \
                $(ROOTSYS)/lib/libMatrix.lib $(ROOTSYS)/lib/libPhysics.lib \
                $(ROOTSYS)/lib/libMathCore.lib \
                $(ROOTSYS)/lib/libNet.lib $(ROOTSYS)/lib/libRIO.lib \
                $(ROOTSYS)/lib/libSpectrum.lib $(ROOTDEV)/lib/libTwoPad.lib \
                $(ROOTDEV)/lib/libSplineFit.lib
#LIBS          = $(ROOTLIBS) $(guilibsdll)
LIBS          = $(ROOTLIBS)
LIBSALL       = $(ROOTLIBS)
LIBNAME       = libLitrani
PROGRAMLIB    = $(LIBNAME).lib

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

PROGRAM    = litrani$(ExeSuf)
PROGRAMSO  = $(LIBNAME).$(DllSuf)
all:        $(PROGRAMSO) $(PROGRAM)

$(PROGRAMSO): $(OBJS)
	@echo "Linking $(PROGRAMSO) ..."
        BINDEXPLIB  $* $(OBJS) > $*.def
        lib -nologo -MACHINE:IX86 $(OBJS) -def:$*.def $(OutPutOpt)$(PROGRAMLIB)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $*.exp $(LIBS) $(OutPutOpt)$(PROGRAMSO)
	@mt -nologo -manifest $(PROGRAMSO).manifest -outputresource:$(PROGRAMSO);2
        @del /q $(PROGRAMSO).manifest
	@echo "$(PROGRAMSO).manifest included into $(PROGRAMSO)"
	@echo "$(PROGRAMSO) done"

$(PROGRAM):  main.$(ObjSuf) $(OBJS)
	@echo "Linking $(PROGRAM) ..."
        $(LD) $(LDFLAGS) main.$(ObjSuf) $(OBJS) $(LIBS) $(OutPutOpt)$(PROGRAM)
	@mt -nologo -manifest $(PROGRAM).manifest -outputresource:$(PROGRAM);1
        @del /q $(PROGRAM).manifest
	@echo "$(PROGRAM).manifest included into $(PROGRAM)"
        @echo "$(PROGRAM) done"

clean:
	@del /q $(OBJS) main.$(ObjSuf) litraniDict.h litraniDict.$(SrcSuf) *~
	@del /q litrani.lib litrani.exp .def
        @del /q $(PROGRAM) $(PROGRAMSO) $(PROGRAMLIB) $(LIBNAME).def $(LIBNAME).exp
        @del /q *.lis *.root
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TCleanOut.h
	@del /q $(ROOTDEV)\include\TLitSpectrumCp.h
	@del /q $(ROOTDEV)\include\TLitSpectrum.h
	@del /q $(ROOTDEV)\include\T33CMatrix.h
	@del /q $(ROOTDEV)\include\T33Matrix.h
	@del /q $(ROOTDEV)\include\T3CVector.h
	@del /q $(ROOTDEV)\include\T3Vector.h
	@del /q $(ROOTDEV)\include\TBeam.h
	@del /q $(ROOTDEV)\include\TSComplex.h
	@del /q $(ROOTDEV)\include\TContact.h
	@del /q $(ROOTDEV)\include\TDetector.h
	@del /q $(ROOTDEV)\include\TElecCascade.h
	@del /q $(ROOTDEV)\include\TEqIndex.h
	@del /q $(ROOTDEV)\include\TFace.h
	@del /q $(ROOTDEV)\include\TLitGlob.h
	@del /q $(ROOTDEV)\include\TMessErr.h
	@del /q $(ROOTDEV)\include\TOpticMaterial.h
	@del /q $(ROOTDEV)\include\TOtherSide.h
	@del /q $(ROOTDEV)\include\TPhoton.h
	@del /q $(ROOTDEV)\include\TPhotonCradle.h
	@del /q $(ROOTDEV)\include\TLitPhys.h
	@del /q $(ROOTDEV)\include\TPol3.h
	@del /q $(ROOTDEV)\include\TPol4.h
	@del /q $(ROOTDEV)\include\TPublication.h
	@del /q $(ROOTDEV)\include\TResults.h
	@del /q $(ROOTDEV)\include\TRevetment.h
	@del /q $(ROOTDEV)\include\TS8PTS.h
	@del /q $(ROOTDEV)\include\TSBRIK.h
	@del /q $(ROOTDEV)\include\TSCONE.h
	@del /q $(ROOTDEV)\include\TSCYL.h
	@del /q $(ROOTDEV)\include\TSGTRA.h
	@del /q $(ROOTDEV)\include\TSNode.h
	@del /q $(ROOTDEV)\include\TSPARA.h
	@del /q $(ROOTDEV)\include\TSParticle.h
	@del /q $(ROOTDEV)\include\TSTRAP.h
	@del /q $(ROOTDEV)\include\TSTRD1.h
	@del /q $(ROOTDEV)\include\TSTRD2.h
	@del /q $(ROOTDEV)\include\TSTUBE.h
	@del /q $(ROOTDEV)\include\TSellmeier.h
	@del /q $(ROOTDEV)\include\TSpontan.h
	@del /q $(ROOTDEV)\include\TStatAPD.h
	@del /q $(ROOTDEV)\include\TStatSurfD.h
	@del /q $(ROOTDEV)\include\TStatVolD.h
	@del /q $(ROOTDEV)\include\TSupplShape.h
	@del /q $(ROOTDEV)\include\TTWave.h
	@del /q $(ROOTDEV)\include\ThinSlice.h
	@del /q $(ROOTDEV)\include\TPhotoElecCompton.h

install:
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TCleanOut.h
	@del /q $(ROOTDEV)\include\TLitSpectrumCp.h
	@del /q $(ROOTDEV)\include\TLitSpectrum.h
	@del /q $(ROOTDEV)\include\T33CMatrix.h
	@del /q $(ROOTDEV)\include\T33Matrix.h
	@del /q $(ROOTDEV)\include\T3CVector.h
	@del /q $(ROOTDEV)\include\T3Vector.h
	@del /q $(ROOTDEV)\include\TBeam.h
	@del /q $(ROOTDEV)\include\TSComplex.h
	@del /q $(ROOTDEV)\include\TContact.h
	@del /q $(ROOTDEV)\include\TDetector.h
	@del /q $(ROOTDEV)\include\TElecCascade.h
	@del /q $(ROOTDEV)\include\TEqIndex.h
	@del /q $(ROOTDEV)\include\TFace.h
	@del /q $(ROOTDEV)\include\TLitGlob.h
	@del /q $(ROOTDEV)\include\TMessErr.h
	@del /q $(ROOTDEV)\include\TOpticMaterial.h
	@del /q $(ROOTDEV)\include\TOtherSide.h
	@del /q $(ROOTDEV)\include\TPhoton.h
	@del /q $(ROOTDEV)\include\TPhotonCradle.h
	@del /q $(ROOTDEV)\include\TLitPhys.h
	@del /q $(ROOTDEV)\include\TPol3.h
	@del /q $(ROOTDEV)\include\TPol4.h
	@del /q $(ROOTDEV)\include\TPublication.h
	@del /q $(ROOTDEV)\include\TResults.h
	@del /q $(ROOTDEV)\include\TRevetment.h
	@del /q $(ROOTDEV)\include\TS8PTS.h
	@del /q $(ROOTDEV)\include\TSBRIK.h
	@del /q $(ROOTDEV)\include\TSCONE.h
	@del /q $(ROOTDEV)\include\TSCYL.h
	@del /q $(ROOTDEV)\include\TSGTRA.h
	@del /q $(ROOTDEV)\include\TSNode.h
	@del /q $(ROOTDEV)\include\TSPARA.h
	@del /q $(ROOTDEV)\include\TSParticle.h
	@del /q $(ROOTDEV)\include\TSTRAP.h
	@del /q $(ROOTDEV)\include\TSTRD1.h
	@del /q $(ROOTDEV)\include\TSTRD2.h
	@del /q $(ROOTDEV)\include\TSTUBE.h
	@del /q $(ROOTDEV)\include\TSellmeier.h
	@del /q $(ROOTDEV)\include\TSpontan.h
	@del /q $(ROOTDEV)\include\TStatAPD.h
	@del /q $(ROOTDEV)\include\TStatSurfD.h
	@del /q $(ROOTDEV)\include\TStatVolD.h
	@del /q $(ROOTDEV)\include\TSupplShape.h
	@del /q $(ROOTDEV)\include\TTWave.h
	@del /q $(ROOTDEV)\include\ThinSlice.h
	@del /q $(ROOTDEV)\include\TPhotoElecCompton.h
	@copy $(PROGRAM) $(ROOTDEV)\bin\$(PROGRAM)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).lib $(ROOTDEV)\lib\$(LIBNAME).lib
	@copy $(LIBNAME).def $(ROOTDEV)\lib\$(LIBNAME).def
	@copy $(LIBNAME).exp $(ROOTDEV)\lib\$(LIBNAME).exp
	@copy TCleanOut.h $(ROOTDEV)\include\TCleanOut.h
	@copy TLitSpectrumCp.h $(ROOTDEV)\include\TLitSpectrumCp.h
	@copy TLitSpectrum.h $(ROOTDEV)\include\TLitSpectrum.h
	@copy T33CMatrix.h $(ROOTDEV)\include\T33CMatrix.h
	@copy T33Matrix.h $(ROOTDEV)\include\T33Matrix.h
	@copy T3CVector.h $(ROOTDEV)\include\T3CVector.h
	@copy T3Vector.h $(ROOTDEV)\include\T3Vector.h
	@copy TBeam.h $(ROOTDEV)\include\TBeam.h
	@copy TSComplex.h $(ROOTDEV)\include\TSComplex.h
	@copy TContact.h $(ROOTDEV)\include\TContact.h
	@copy TDetector.h $(ROOTDEV)\include\TDetector.h
	@copy TElecCascade.h $(ROOTDEV)\include\TElecCascade.h
	@copy TEqIndex.h $(ROOTDEV)\include\TEqIndex.h
	@copy TFace.h $(ROOTDEV)\include\TFace.h
	@copy TLitGlob.h $(ROOTDEV)\include\TLitGlob.h
	@copy TMessErr.h $(ROOTDEV)\include\TMessErr.h
	@copy TOpticMaterial.h $(ROOTDEV)\include\TOpticMaterial.h
	@copy TOtherSide.h $(ROOTDEV)\include\TOtherSide.h
	@copy TPhoton.h $(ROOTDEV)\include\TPhoton.h
	@copy TPhotonCradle.h $(ROOTDEV)\include\TPhotonCradle.h
	@copy TLitPhys.h $(ROOTDEV)\include\TLitPhys.h
	@copy TPol3.h $(ROOTDEV)\include\TPol3.h
	@copy TPol4.h $(ROOTDEV)\include\TPol4.h
	@copy TPublication.h $(ROOTDEV)\include\TPublication.h
	@copy TResults.h $(ROOTDEV)\include\TResults.h
	@copy TRevetment.h $(ROOTDEV)\include\TRevetment.h
	@copy TS8PTS.h $(ROOTDEV)\include\TS8PTS.h
	@copy TSBRIK.h $(ROOTDEV)\include\TSBRIK.h
	@copy TSCONE.h $(ROOTDEV)\include\TSCONE.h
	@copy TSCYL.h $(ROOTDEV)\include\TSCYL.h
	@copy TSGTRA.h $(ROOTDEV)\include\TSGTRA.h
	@copy TSNode.h $(ROOTDEV)\include\TSNode.h
	@copy TSPARA.h $(ROOTDEV)\include\TSPARA.h
	@copy TSParticle.h $(ROOTDEV)\include\TSParticle.h
	@copy TSTRAP.h $(ROOTDEV)\include\TSTRAP.h
	@copy TSTRD1.h $(ROOTDEV)\include\TSTRD1.h
	@copy TSTRD2.h $(ROOTDEV)\include\TSTRD2.h
	@copy TSTUBE.h $(ROOTDEV)\include\TSTUBE.h
	@copy TSellmeier.h $(ROOTDEV)\include\TSellmeier.h
	@copy TSpontan.h $(ROOTDEV)\include\TSpontan.h
	@copy TStatAPD.h $(ROOTDEV)\include\TStatAPD.h
	@copy TStatSurfD.h $(ROOTDEV)\include\TStatSurfD.h
	@copy TStatVolD.h $(ROOTDEV)\include\TStatVolD.h
	@copy TSupplShape.h $(ROOTDEV)\include\TSupplShape.h
	@copy TTWave.h $(ROOTDEV)\include\TTWave.h
	@copy ThinSlice.h $(ROOTDEV)\include\ThinSlice.h
	@copy TPhotoElecCompton.h $(ROOTDEV)\include\TPhotoElecCompton.h
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

litraniDict.$(SrcSuf): $(HDRS)
	@echo "Deleting Dictionary..."
	@del /q litraniDict.h litraniDict.$(SrcSuf) litraniDict.$(ObjSuf)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)\bin\rootcint -f litraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h

.$(SrcSuf).$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -c $<
