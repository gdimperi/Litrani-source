#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TMessErr+;
#pragma link C++ class TCleanOut+;
#pragma link C++ class TLitPhys+;
#pragma link C++ class TLitSpectrumCp+;
#pragma link C++ class TLitSpectrum+;
#pragma link C++ class TEqIndex+;
#pragma link C++ class TLitGlob+;
#pragma link C++ class TSComplex+;
#pragma link C++ class TPol3+;
#pragma link C++ class TPol4+;
#pragma link C++ class TSellmeier+;
#pragma link C++ class T33Matrix+;
#pragma link C++ class T33CMatrix+;
#pragma link C++ class T3Vector+;
#pragma link C++ class T3CVector+;
#pragma link C++ class TOpticMaterial+;
#pragma link C++ class TRevetment+;
#pragma link C++ class ThinSlice+;
#pragma link C++ class TSNode+;
#pragma link C++ class TFace+;
#pragma link C++ class TSupplShape+;
#pragma link C++ class TSBRIK+;
#pragma link C++ class TSTRD1+;
#pragma link C++ class TSTRD2+;
#pragma link C++ class TSPARA+;
#pragma link C++ class TSTRAP+;
#pragma link C++ class TSGTRA+;
#pragma link C++ class TS8PTS+;
#pragma link C++ class TSCYL+;
#pragma link C++ class TSTUBE+;
#pragma link C++ class TSCONE+;
#pragma link C++ class TOtherSide+;
#pragma link C++ class TContact+;
#pragma link C++ class TResults+;
#pragma link C++ class TTWave+;
#pragma link C++ class TPhoton+;
#pragma link C++ class TPhotonCradle+;
#pragma link C++ class TSpontan+;
#pragma link C++ class TDetector+;
#pragma link C++ class TStatSurfD+;
#pragma link C++ class TStatVolD+;
#pragma link C++ class TStatAPD+;
#pragma link C++ class TSParticle+;
#pragma link C++ class TBeam+;
#pragma link C++ class TElecCascade+;
#pragma link C++ class TPublication+;
#pragma link C++ class TPhotoElecCompton+;


#pragma link C++ enum KindOfFace;
#pragma link C++ enum KindOfMes;
#pragma link C++ enum KindOfShape;
#pragma link C++ enum KindOfContact;
#pragma link C++ enum KindOfCradle;
#pragma link C++ enum KindOfDist;
#pragma link C++ enum ChoiceOfn;
#pragma link C++ enum PDistribution;
#pragma link C++ enum OpticalProcess;

#pragma link C++ global gLit;
#pragma link C++ global gGs;
#pragma link C++ global gCs;
#pragma link C++ global gGp;
#pragma link C++ global gLitPhys;
#pragma link C++ global gOutside;
#pragma link C++ global gIdx;
#pragma link C++ global gRandom3;
#pragma link C++ global gMes;

#pragma link C++ function operator+(const Double_t,const TSComplex&);
#pragma link C++ function operator-(const Double_t,const TSComplex&);
#pragma link C++ function operator*(const Double_t,const TSComplex&);
#pragma link C++ function operator*(const TSComplex&,const TSComplex&);
#pragma link C++ function operator/(const Double_t,const TSComplex&);
#pragma link C++ function operator>>(istream&,TSComplex&);
#pragma link C++ function operator<<(ostream&,const TSComplex&);
#pragma link C++ function Abs(TSComplex&);
#pragma link C++ function Sqrt(TSComplex&);
#pragma link C++ function Exp(TSComplex&);
#pragma link C++ function Log(TSComplex&);
#pragma link C++ function Sin(TSComplex&);
#pragma link C++ function Cos(TSComplex&);

#pragma link C++ function operator&(const T33Matrix&,const T33Matrix&);
#pragma link C++ function operator*(const T33Matrix&,const T33Matrix&);
#pragma link C++ function operator*(const Double_t,  const T33Matrix&);
#pragma link C++ function operator*(const T33Matrix&,const Double_t);

#pragma link C++ function operator*(const T3Vector&,  const T3Vector&);
#pragma link C++ function operator*(const Double_t,   const T3Vector&);
#pragma link C++ function operator*(const T3Vector&,  const Double_t);
#pragma link C++ function operator*(const T33Matrix&, const T3Vector&);
#pragma link C++ function operator*(const T3Vector&,  const T33Matrix&);
#pragma link C++ function operator>>(istream&,T3Vector&);
#pragma link C++ function operator<<(ostream&,const T3Vector&);


#endif
