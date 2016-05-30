// @(#)root/html:$Id: TPublication.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TwoPadDisplay.h"
#include "TLitGlob.h"
#include "TMessErr.h"
#include "TResults.h"
#include "TStatSurfD.h"
#include "TStatVolD.h"
#include "TStatAPD.h"
#include "TPublication.h"

TPublication* gGp = 0;

ClassImp(TPublication)
//______________________________________________________________________________
//
//  TPublication helps in making the final analysis, after all runs have
//been produced. For a general presentation of statistics and histograms
//in Litrani, look first at the class description of TResults.
//
//  When you are finished with the generation of all runs, comes the time to
//analyze the results produced by Litrani. In what concerns the results PER
//RUN, look at the class TResults, where everything is explained on how to
//look at the histograms of each run or at the histograms which are the
//summation of all runs.
//  But you may also be interested in seeing how some physical quantity va-
//ries as a function of run, or rather as a function of the parameter you
//have changed from run to run. For instance, may be you have varied the z
//position of the beam from run to run, z being along the long axis of some
//crystal, and you want to know how the efficiency for detecting photons
//with an APD varies with the position of the beam. This is for this type of
//analysis that the class TPublication is for. Even in the case where the
//type of analysis done by TPublication does not suit your needs, it is
//interesting for you to look at the code of TPublication, in order to learn
//how to extract physical informations from Litrani.
//  The analysis done in TPublication provide you with the following :
//    (a) - an histogram giving the variation of the efficiency as a function
//of the parameter you have varied from run to run. Look at the method
//DrawEfficiency();
//    (b) - many histograms giving the variation of all kinds of losses as a
//function of the parameter you have varied from run to run. Look at all the
//methods beginning with Draw...
//    (c) - the possibility to look at the summary histograms of each detec-
//tor, for instance the histogram giving the variation of the efficiency
//OF SOME SPECIFIC DETECTOR as a function of the parameter you have varied
//from run to run. These summary histograms per detector are contained in
//the classes TStatSurfD, TStatVolD, TStatAPD.
//    (d) - the method N() allows you to look at all summary histograms in
//turn, even the summary histograms of each detector.
//
//  The class TPublication is pointed to by the global pointer gGp. The user
//has not to define this pointer, it has already been done in the method
//Gen() of class TBeam or TSpontan. But there is a good reason for the
//user to change the title (second argument of the constructor of the class
//TPublication) that was given when creating gGp. We will explain why :
//  When you have called the method Gen(), TSpontan::Gen() or TBeam::Gen() or
//TElecCascade::Gen(), you had the possibility to give a free parameter
//xparam, intended to be used as the x axis parameter in the summary histo-
//grams. For instance, may be you have varied the z position of the beam
//from run to run, z being along the long axis of some crystal, and you want
//to know how the efficiency for detecting photons with an APD varies with
//the position of the beam. In that case, you have given as xparam when
//calling Gen() the value of the z position of the beam. It would be nice if
//the title of each summary histogram and the label of the x axis of these
//histograms would contain "z coordinate of beam". This is possible by
//giving "z coordinate of beam" as argument "title" for gGp. You can do
//that by :
//    gGp->SetTitle("z coordinate of beam");
//
//  For instance, the histogram fHEfficiency has for original title
//"Efficiency versus ". To this original title will be appended the title of
//gGp, so that the final title will be :
//      "Efficiency versus z coordinate of beam".
//The same process apply for all summary histograms.
//
//
//  USING gGp
//
//  At the end of the production of all runs, do the following inside the
//a macro or directly on the keyboard :
//
//    gGp->SetTitle("z coordinate of beam");
//    gGp->Summary();
//    gLit->CloseFiles();
//
//  Summary() will book and fill all summary histograms. You are now ready
//to look at the histograms.
//
//  (A) - If all you want to do is looking in turn at each histogram,
//including the detector summary histograms, simply do :
//
//    gGp->N();   //You will then see the 1st histogram. To see the next one,
//                  simply type n<CR>. To stop the show, type any key, for
//                  instance q<CR>.
//  (B) - If you want to see a particular summary histogram, use one of the
//Draw method, for instance :
//
//    gGp->DrawAbsMat();
//
//  (C) - If you want to see all losses superimposed onto one histogram, do :
//
//    fGp->DrawAllLosses();
//         
//        in fact, you will get 2 plots, one in linear y scale, the other
//        in logarithmic y scale.
//
//  (D) - If you want to see a particular summary detector histogram, you
//must know the kind and the name of the detector. The detectors may be of 4
//kinds:
//    (a) - General surface detectors, contained in the class TStatSurfD
//    (b) - Phototubes, also contained in the class TStatSurfD
//    (c) - general volume detectors, contained in the class TStatVolD
//    (d) - APD contained in the class TStatAPD.
//  For more informations on detectors, look at the class TSupplShape.
//  TPublication provides you with 3 methods to reach the summary histograms
//of some detector : GetPSurf(name), GetPVol(name) and GetPAPD(name). The
//use of these methods implies that 
//              (1) you know the type (a)...(d) of the wanted detector [ you
//have to call GetPSurf() for type (a) or (b), GetPVol() for type (c),
//and GetPAPD() for type (d) ]
//              (2) you know the name of the wanted detector [ each detec-
//tor in Litrani must have a different name ].
//  For instance, if you want to see the efficiency of an APD named "hama-
//matsu" as a function of the parameter xparam, do :
//
//    TStatAPD *papd = gGp->GetPAPD("hamamatsu");
//    papd->fHEfficiency->Draw();
//
TPublication::TPublication(const char *name,const char *title, Bool_t inmem) 
: TNamed(name,title) {
  //
  //  The constructor of TPublication is called the first time a Gen() method
  //is called [ TSpontan::Gen() or TBeam::Gen() ]. The user of Litrani has not
  //to bother about calling the constructor of this class.
  //
  //  The parameters are the following :
  //
  //  name   : arbitrary name.
  //  title  : if you have provided xparam when calling Gen() [ See
  //           TSpontan::Gen() or TBeam::Gen() ], i.e. if the final results
  //           have to be presented as a function of the parameter xparam
  //           used as abscissa, the title is the description of this para-
  //           meter. For instance, title could be :
  //               "Incident angle of fibre" or
  //               "Beam position along crystal axis", and so on.
  //           Otherwise, define title for instance as :
  //               "Run number"
  //  inmem  : the statistics of all run is in memory. TPublication will not
  //           work otherwise.
  //
  fInMem    = inmem;
  fK        = 0;
  fNDet     = 0;
  fNHperDet = 1;
  InitP();
}
TPublication::~TPublication() {
  if (fHEfficiency) {
    delete fHEfficiency;
    fHEfficiency = 0;
  }
  if (fHAbsMat) {
    delete fHAbsMat;
    fHAbsMat = 0;
  }
  if (fHAbsBef) {
    delete fHAbsBef;
    fHAbsBef = 0;
  }
  if (fHAbsRvt) {
    delete fHAbsRvt;
    fHAbsRvt = 0;
  }
  if (fHOutSide) {
    delete fHOutSide;
    fHOutSide = 0;
  }
  if (fHAbnorm) {
    delete fHAbnorm;
    fHAbnorm = 0;
  }
  if (fHTooLate) {
    delete fHTooLate;
    fHTooLate = 0;
  }
  if (fHTooFew) {
    delete fHTooFew;
    fHTooFew = 0;
  }
  if (fHLossAng) {
    delete fHLossAng;
    fHLossAng = 0;
  }
  if (fHLossQE) {
    delete fHLossQE;
    fHLossQE = 0;
  }
}
void TPublication::BookHistos() {
  //Booking of all histograms
  const Axis_t z05 = 0.5;
  Int_t i,j,n,krun;
  Axis_t d2;
  Axis_t *xmid, *xlow;
  n = gLit->fNResults;
  if (n != gLit->fResults.GetEntries()) {
    gMes->SetName("BookHistos");
    gMes->SetTitle("TPublication");
    gMes->M(Error_M,1,"bad fNResults");
  }
  fNbins = n - 1;
  xmid = new Axis_t[fNbins];
  xlow = new Axis_t[n];
  TResults *p;
  TIter next(&gLit->fResults);
  i = 0;
  while ((p = (TResults *)next())) {
    krun  = p->GetRunNumber();
    if (krun != 0) {
      xmid[i] = p->fXparam;
      i += 1;
    }
  }
  if (fNbins == 1) {
    xlow[0] = xmid[0] - z05;
    xlow[1] = xmid[0] + z05;
  }
  else {
    for (i=1;i<fNbins;i++) {
      j  = i - 1;
      d2 = z05*(xmid[i] - xmid[j]);
      xlow[i] = xmid[j] + d2;
      if (i==1) xlow[j] = xmid[j] - d2;
      if (i==fNbins-1) xlow[fNbins] = xmid[fNbins-1] + d2;
    }
  }
  delete [] xmid;
  TString ssd;
  TString fullname,fulltitle,title;
  title = GetTitle();
  fulltitle = "Efficiency versus ";
  fulltitle.Append(title);
  fHEfficiency = new TH1F("Efficiency_pr",fulltitle.Data(),fNbins,xlow);
  fHEfficiency->GetXaxis()->SetTitleSize(0.035);
  fHEfficiency->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost inside materials versus ";
  fulltitle.Append(title);
  fHAbsMat = new TH1F("AbsMat_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsMat->GetXaxis()->SetTitleSize(0.035);
  fHAbsMat->SetXTitle((char *)title.Data());
  //
  fulltitle = "Absorbed before revetment versus ";
  fulltitle.Append(title);
  fHAbsBef = new TH1F("AbsBef_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsBef->GetXaxis()->SetTitleSize(0.035);
  fHAbsBef->SetXTitle((char *)title.Data());
  //
  fulltitle = "Absorbed by revetment versus ";
  fulltitle.Append(title);
  fHAbsRvt = new TH1F("AbsRvt_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsRvt->GetXaxis()->SetTitleSize(0.035);
  fHAbsRvt->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because leaving the setup versus ";
  fulltitle.Append(title);
  fHOutSide = new TH1F("OutSide_pr",fulltitle.Data(),fNbins,xlow);
  fHOutSide->GetXaxis()->SetTitleSize(0.035);
  fHOutSide->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost for abnormal reasons versus ";
  fulltitle.Append(title);
  fHAbnorm = new TH1F("Abnorm_pr",fulltitle.Data(),fNbins,xlow);
  fHAbnorm->GetXaxis()->SetTitleSize(0.035);
  fHAbnorm->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because seen too late versus ";
  fulltitle.Append(title);
  fHTooLate = new TH1F("TooLate_pr",fulltitle.Data(),fNbins,xlow);
  fHTooLate->GetXaxis()->SetTitleSize(0.035);
  fHTooLate->SetXTitle((char *)title.Data());
  //
  fulltitle = "Too few electrons in APD versus ";
  fulltitle.Append(title);
  fHTooFew = new TH1F("TooFew_pr",fulltitle.Data(),fNbins,xlow);
  fHTooFew->GetXaxis()->SetTitleSize(0.035);
  fHTooFew->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because of acceptance angle versus ";
  fulltitle.Append(title);
  fHLossAng = new TH1F("LossAng_pr",fulltitle.Data(),fNbins,xlow);
  fHLossAng->GetXaxis()->SetTitleSize(0.035);
  fHLossAng->SetXTitle((char *)title.Data());
  //
  fulltitle = "Quantum Eff. losses (except APD) versus ";
  fulltitle.Append(title);
  fHLossQE = new TH1F("LossQE_pr",fulltitle.Data(),fNbins,xlow);
  fHLossQE->GetXaxis()->SetTitleSize(0.035);
  fHLossQE->SetXTitle((char *)title.Data());
  //
  //Histograms of all surface detectors detectors
  TResults *pglob;
  pglob = GlobalP();
  if (pglob) {
    //Summary Histograms of all surface detectors
    if (pglob->fNSurfDet) {
      i=0;
      TStatSurfD *psurf;
      TIter nexts(&pglob->fSurfDet);
      while ((psurf = (TStatSurfD *)nexts())) {
        TString s1 = "";
        s1 += i;
        fullname = "Efficiency_s";
        fullname.Append(s1);
        if (psurf->IsPhototube()) ssd = "phototube";
        else                      ssd = "surface detector";
        fulltitle = "Efficiency ";
        fulltitle.Append(ssd);
        fulltitle.Append(s1);
        fulltitle.Append(" ");
        fulltitle.Append(psurf->GetName());
        fulltitle.Append(" versus ");
        fulltitle.Append(title);
        if (psurf->fHEfficiency) {
          delete psurf->fHEfficiency;
          psurf->fHEfficiency = 0;
        }
        psurf->fHEfficiency = new TH1F(fullname.Data(),fulltitle.Data(),
          fNbins,xlow);
        psurf->fHEfficiency->GetXaxis()->SetTitleSize(0.035);
        psurf->fHEfficiency->SetXTitle((char *)title.Data());
        i += 1;
      }//end while (psurf = (TStatSurfD *)nexts())
    }//end if (pglob->fNSurfDet)
    //Summary Histograms of all volume detectors
    if (pglob->fNVolDet) {
      i=0;
      TStatVolD *pvol;
      TIter nextv(&pglob->fVolDet);
      while ((pvol = (TStatVolD *)nextv())) {
        TString s1 = "";
        s1 += i;
        fullname = "Efficiency_v";
        fullname.Append(s1);
        fulltitle = "Efficiency vol. det. ";
        fulltitle.Append(s1);
        fulltitle.Append(" ");
        fulltitle.Append(pvol->GetName());
        fulltitle.Append(" versus ");
        fulltitle.Append(title);
        if (pvol->fHEfficiency) {
          delete pvol->fHEfficiency;
          pvol->fHEfficiency = 0;
        }
        pvol->fHEfficiency = new TH1F(fullname.Data(),fulltitle.Data(),
          fNbins,xlow);
        pvol->fHEfficiency->GetXaxis()->SetTitleSize(0.035);
        pvol->fHEfficiency->SetXTitle((char *)title.Data());
        i += 1;
      }//end while (pvol = (TStatVolD *)nextv())
    }//end if (pglob->fNVolDet)
    //Summary Histograms of all APD detectors
    if (pglob->fNAPD) {
      i=0;
      TStatAPD *papd;
      TIter nexta(&pglob->fAPD);
      while ((papd = (TStatAPD *)nexta())) {
        TString s1 = "";
        s1 += i;
        fullname = "Efficiency_a";
        fullname.Append(s1);
        fulltitle = "Efficiency APD ";
        fulltitle.Append(s1);
        fulltitle.Append(" ");
        fulltitle.Append(papd->GetName());
        fulltitle.Append(" versus ");
        fulltitle.Append(title);
        if (papd->fHEfficiency) {
          delete papd->fHEfficiency;
          papd->fHEfficiency = 0;
        }
        papd->fHEfficiency = new TH1F(fullname.Data(),fulltitle.Data(),
          fNbins,xlow);
        papd->fHEfficiency->GetXaxis()->SetTitleSize(0.035);
        papd->fHEfficiency->SetXTitle((char *)title.Data());
        i += 1;
      }//end while (papd = (TStatAPD *)nexta())
    }//end if (pglob->fNAPD)
    fNDet = pglob->fNSurfDet + pglob->fNVolDet + pglob->fNAPD;
  }//end if (pglob)
  delete [] xlow;
}
void TPublication::D(Int_t kdet,Int_t kh) const {
  //draw histogram kh of detector kdet
  Int_t det1;
  TResults *pglob;
  if (gGs) pglob = gGs;
  else     pglob = GlobalP();
  if (!gTwoPad) gLit->BookCanvas();
  kdet %= fNDet;
  if (kdet<pglob->fNSurfDet) {
    //It is a surface detector
    TStatSurfD *gsurf;
    gsurf = (TStatSurfD *)pglob->fSurfDet[kdet];
    switch (kh) {
    default:
      gsurf->fHEfficiency->Draw("E1");
      break;
    }
  }//end if (kdet<pglob->fNSurfDet)
  else {
    det1 = kdet - pglob->fNSurfDet;
    if (det1<pglob->fNVolDet) {
      //It is a volume detector
      TStatVolD *gvol;
      gvol = (TStatVolD *)pglob->fVolDet[det1];
      switch (kh) {
      default:
        gvol->fHEfficiency->Draw("E1");
        break;
      }
    }//end if (det1<pglob->fNVolDet)
    else {
      //It is an APD
      det1 -= pglob->fNVolDet;
      TStatAPD *gapd;
      gapd = (TStatAPD *)pglob->fAPD[det1];
      switch (kh) {
      default:
        gapd->fHEfficiency->Draw("E1");
        break;
      }
    }//end else (det1<pglob->fNVolDet)
  }//end else (kdet<pglob->fNSurfDet)
}
void TPublication::DrawAbnorm() const {
  //draws histogram fHAbnorm
  gTwoPad->SetNameForSave(fHAbnorm->GetName());
  fHAbnorm->Draw("E1");
}
void TPublication::DrawAbsBef() const {
  //draws histogram fHAbsBef
  gTwoPad->SetNameForSave(fHAbsBef->GetName());
  fHAbsBef->Draw("E1");
}
void TPublication::DrawAbsMat() const {
  //draws histogram fHAbsMat
  gTwoPad->SetNameForSave(fHAbsMat->GetName());
  fHAbsMat->Draw("E1");
}
void TPublication::DrawAbsRvt() const {
  //draws histogram fHAbsRvt
  gTwoPad->SetNameForSave(fHAbsRvt->GetName());
  fHAbsRvt->Draw("E1");
}
void TPublication::DrawAllLosses() const {
  //Draw all losses, superimposed.
  char c='n';
  Int_t n;
  gTwoPad->SelectPad1();
  fHAbsMat->SetMaximum(1.0);
  fHAbsMat->Draw();
  fHAbsBef->Draw("SAME");
  fHAbsRvt->Draw("SAME");
  fHOutSide->Draw("SAME");
  fHAbnorm->Draw("SAME");
  fHTooLate->Draw("SAME");
  fHTooFew->Draw("SAME");
  fHLossAng->Draw("SAME");
  fHLossQE->Draw("SAME");
  gTwoPad->SelectPad2();
  gTwoPad->SetPadLogY(0);
  fHAbsMat->SetMinimum(0.001);
  fHAbsMat->Draw();
  n = Int_t(fHAbsBef->GetEntries());
  if (n) fHAbsBef->Draw("SAME");
  n = Int_t(fHAbsRvt->GetEntries());
  if (n) fHAbsRvt->Draw("SAME");
  n = Int_t(fHOutSide->GetEntries());
  if (n) fHOutSide->Draw("SAME");
  n = Int_t(fHAbnorm->GetEntries());
  if (n) fHAbnorm->Draw("SAME");
  n = Int_t(fHTooLate->GetEntries());
  if (n) fHTooLate->Draw("SAME");
  n = Int_t(fHTooFew->GetEntries());
  if (n) fHTooFew->Draw("SAME");
  n = Int_t(fHLossAng->GetEntries());
  if (n) fHLossAng->Draw("SAME");
  n = Int_t(fHLossQE->GetEntries());
  if (n) fHLossQE->Draw("SAME");
  gTwoPad->UpdatePad1();
  gTwoPad->UpdatePad2();
  cout << "Press  any key to continue" << endl;
  cin >> c;
  fHAbsMat->SetMinimum(0.0);
}
void TPublication::DrawEfficiency() const {
  //draws histogram fHEfficiency
  gTwoPad->SetNameForSave(fHEfficiency->GetName());
  fHEfficiency->Draw("E1");
}
void TPublication::DrawLossAng() const {
  //draws histogram fHLossAng
  gTwoPad->SetNameForSave(fHLossAng->GetName());
  fHLossAng->Draw("E1");
}
void TPublication::DrawLossQE() const {
  //draws histogram fHLossQE
  gTwoPad->SetNameForSave(fHLossQE->GetName());
  fHLossQE->Draw("E1");
}
void TPublication::DrawOutSide() const {
  //draws histogram fHOutSide
  gTwoPad->SetNameForSave(fHOutSide->GetName());
  fHOutSide->Draw("E1");
}
void TPublication::DrawTooFew() const {
  //draws histogram fHTooFew
  gTwoPad->SetNameForSave(fHTooFew->GetName());
  fHTooFew->Draw("E1");
}
void TPublication::DrawTooLate() const {
  //draws histogram fHTooLate
  gTwoPad->SetNameForSave(fHTooLate->GetName());
  fHTooLate->Draw("E1");
}
void TPublication::FillHistos() {
  //Filling of all histograms
  const Int_t NbOfHistos = 10;
  Int_t i,j,krun;
  Double_t num,den;
  Int_t los,bil;
  Stat_t y,dy;
  Bool_t b[NbOfHistos];
  for (i=0;i<NbOfHistos;i++) b[i] = kFALSE;
  TResults *p,*pglob;
  TString s;
  s = GetTitle();
  pglob = GlobalP();
  TIter next(&gLit->fResults);
  i=0;
  while ((p = (TResults *)next())) {
    krun  = p->GetRunNumber();
    if (krun != 0) {
      i   += 1;
      if (p->fNpSeen) b[0] = kTRUE;
      num  = p->fNpSeen;
      den  = p->fNpGener - p->fNpAbnorm - p->fNpAbsL0;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHEfficiency->SetBinContent(i,y);
      fHEfficiency->SetBinError(i,dy);
      los  = p->fNpAbsMat;
      if (p->fNpAbsMat) b[1] = kTRUE;
      num  = p->fNpAbsMat;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHAbsMat->SetBinContent(i,y);
      fHAbsMat->SetBinError(i,dy);
      los += p->fNpAbsBef;
      if (p->fNpAbsBef) b[2] = kTRUE;
      num  = p->fNpAbsBef;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHAbsBef->SetBinContent(i,y);
      fHAbsBef->SetBinError(i,dy);
      los += p->fNpAbsRvt;
      if (p->fNpAbsRvt) b[3] = kTRUE;
      num  = p->fNpAbsRvt;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHAbsRvt->SetBinContent(i,y);
      fHAbsRvt->SetBinError(i,dy);
      los += p->fNpOutSide;
      if (p->fNpOutSide) b[4] = kTRUE;
      num  = p->fNpOutSide;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHOutSide->SetBinContent(i,y);
      fHOutSide->SetBinError(i,dy);
      los += p->fNpAbnorm;
      if (p->fNpAbnorm) b[5] = kTRUE;
      num  = p->fNpAbnorm;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHAbnorm->SetBinContent(i,y);
      fHAbnorm->SetBinError(i,dy);
      los += p->fNpTooLate;
      if (p->fNpTooLate) b[6] = kTRUE;
      num  = p->fNpTooLate;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHTooLate->SetBinContent(i,y);
      fHTooLate->SetBinError(i,dy);
      los += p->fNpTooFew;
      if (p->fNpTooFew) b[7] = kTRUE;
      num  = p->fNpTooFew;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHTooFew->SetBinContent(i,y);
      fHTooFew->SetBinError(i,dy);
      los += p->fNpLossAng;
      if (p->fNpLossAng) b[8] = kTRUE;
      num  = p->fNpLossAng;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHLossAng->SetBinContent(i,y);
      fHLossAng->SetBinError(i,dy);
      los += p->fNpLossQE;
      if (p->fNpLossQE) b[9] = kTRUE;
      num  = p->fNpLossQE;
      y    = num/den;
      dy   = TMath::Sqrt(num)/den;
      fHLossQE->SetBinContent(i,y);
      fHLossQE->SetBinError(i,dy);
      //Summary Histograms of all surface detectors
      if (pglob->fNSurfDet) {
        TStatSurfD *psurf,*gsurf;
        for (j=0;j<pglob->fNSurfDet;j++) {
          psurf = (TStatSurfD *)p->fSurfDet[j];
          gsurf = (TStatSurfD *)pglob->fSurfDet[j];
          num   = psurf->fNpSeen;
          y     = num/den;
          dy    = TMath::Sqrt(num)/den;
          gsurf->fHEfficiency->SetBinContent(i,y);
          gsurf->fHEfficiency->SetBinError(i,dy);
        }//end for (j=0;j<pglob->fNSurfDet;j++)
      }//end if (pglob->fNSurfDet)
      //Summary Histograms of all volume detectors
      if (pglob->fNVolDet) {
        TStatVolD *pvol,*gvol;
        for (j=0;j<pglob->fNVolDet;j++) {
          pvol = (TStatVolD *)p->fVolDet[j];
          gvol = (TStatVolD *)pglob->fVolDet[j];
          num   = pvol->fNpSeen;
          y     = num/den;
          dy    = TMath::Sqrt(num)/den;
          gvol->fHEfficiency->SetBinContent(i,y);
          gvol->fHEfficiency->SetBinError(i,dy);
        }//end for (j=0;j<pglob->fNSurfDet;j++)
      }//end if (pglob->fNSurfDet)
      //Summary Histograms of all apd detectors
      if (pglob->fNAPD) {
        TStatAPD *papd,*gapd;
        for (j=0;j<pglob->fNAPD;j++) {
          papd = (TStatAPD *)p->fAPD[j];
          gapd = (TStatAPD *)pglob->fAPD[j];
          num   = papd->fNpSeen;
          y     = num/den;
          dy    = TMath::Sqrt(num)/den;
          gapd->fHEfficiency->SetBinContent(i,y);
          gapd->fHEfficiency->SetBinError(i,dy);
        }//end for (j=0;j<pglob->fNSurfDet;j++)
      }//end if (pglob->fNSurfDet)
      //Tests
      bil  = p->fNpGener - p->fNpAbsL0 - p->fNpSeen - los;
      if (bil) {
        gMes->SetName("FillHistos");
        gMes->SetTitle("TPublication");
        gMes->M(Error_M,1,"Balance of photons bad");
      }
    }
  }
  //  fHEfficiency->Fit("pol2","Q");
  //  fHAbsMat->Fit("pol2","Q");
  if (b[1]) fHAbsMat->SetEntries(1);
  //  fHAbsBef->Fit("pol2","Q");
  if (b[2]) fHAbsBef->SetEntries(1);
  //  fHAbsRvt->Fit("pol2","Q");
  if (b[3]) fHAbsRvt->SetEntries(1);
  //  fHOutSide->Fit("pol2","Q");
  if (b[4]) fHOutSide->SetEntries(1);
  //  fHAbnorm->Fit("pol2","Q");
  if (b[5]) fHAbnorm->SetEntries(1);
  //  fHTooLate->Fit("pol2","Q");
  if (b[6]) fHTooLate->SetEntries(1);
  //  fHTooFew->Fit("pol2","Q");
  if (b[7]) fHTooFew->SetEntries(1);
  //  fHLossAng->Fit("pol2","Q");
  if (b[8]) fHLossAng->SetEntries(1);
  //  fHLossQE->Fit("pol2","Q");
  if (b[9]) fHLossQE->SetEntries(1);
  //Summary Histograms of all surface detectors
  //  if (pglob->fNSurfDet) {
  //    TStatSurfD *gsurf;
  //    for (j=0;j<pglob->fNSurfDet;j++) {
  //      gsurf = (TStatSurfD *)pglob->fSurfDet[j];
  //      gsurf->fHEfficiency->Fit("pol2","Q");
  //    }//end for (j=0;j<pglob->fNSurfDet;j++)
  //  }//end if (pglob->fNSurfDet)
  //Summary Histograms of all volume detectors
  //  if (pglob->fNVolDet) {
  //    TStatVolD *gvol;
  //    for (j=0;j<pglob->fNVolDet;j++) {
  //      gvol = (TStatVolD *)pglob->fVolDet[j];
  //      gvol->fHEfficiency->Fit("pol2","Q");
  //    }//end for (j=0;j<pglob->fNVolDet;j++)
  //  }//end if (pglob->fNVolDet)
  //Summary Histograms of all apd detectors
  //  if (pglob->fNAPD) {
  //    TStatAPD *gapd;
  //    for (j=0;j<pglob->fNAPD;j++) {
  //      gapd = (TStatAPD *)pglob->fAPD[j];
  //      gapd->fHEfficiency->Fit("pol2","Q");
  //    }//end for (j=0;j<pglob->fNVolDet;j++)
  //  }//end if (pglob->fNVolDet)
}
TStatAPD* TPublication::GetPAPD(Int_t kdet) const {
  //Get the pointer onto the APD detector kdet in order to reach the
  //summary histograms of this detector. Notice that detectors are numbered
  //beginning with 0 so that kdet < number of APD.
  TResults *pglob;
  TStatAPD *p;
  pglob = GlobalP();
  kdet %= pglob->fNAPD;
  p = (TStatAPD *)pglob->fAPD[kdet];
  return p;
}
TStatAPD* TPublication::GetPAPD(const char *name) const {
  //Get the pointer onto the APD detector whose name is "name" in order to
  //reach the summary histograms of this detector.
  Bool_t found = kFALSE;
  TResults *pglob;
  TString s;
  TStatAPD *p;
  TStatAPD *papd=0;
  pglob = GlobalP();
  TIter next(&pglob->fAPD);
  while ((!found) && (p = (TStatAPD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      papd = p;
    } 
  }
  return papd;
}
TStatSurfD* TPublication::GetPSurf(Int_t kdet) const {
  //Get the pointer onto the surface detector kdet in order to reach the
  //summary histograms of this detector. Notice that detectors are numbered
  //beginning with 0 so that kdet < number of surface detectors.
  TResults *pglob;
  TStatSurfD *p;
  pglob = GlobalP();
  kdet %= pglob->fNSurfDet;
  p = (TStatSurfD *)pglob->fSurfDet[kdet];
  return p;
}
TStatSurfD* TPublication::GetPSurf(const char *name) const {
  //Get the pointer onto the surface detector whose name is "name" in order to
  //reach the summar histograms of this detector.
  Bool_t found = kFALSE;
  TResults *pglob;
  TString s;
  TStatSurfD *p;
  TStatSurfD *psurf=0;
  pglob = GlobalP();
  TIter next(&pglob->fSurfDet);
  while ((!found) && (p = (TStatSurfD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      psurf = p;
    } 
  }
  return psurf;
}
TStatVolD* TPublication::GetPVol(Int_t kdet) const {
  //Get the pointer onto the volume detector kdet in order to reach the
  //summary histograms of this detector. Notice that detectors are numbered
  //beginning with 0 so that kdet < number of volume detectors.
  TResults *pglob;
  TStatVolD *p;
  pglob = GlobalP();
  kdet %= pglob->fNVolDet;
  p = (TStatVolD *)pglob->fVolDet[kdet];
  return p;
}
TStatVolD* TPublication::GetPVol(const char *name) const {
  //Get the pointer onto the volume detector  whose name is "name"  in order
  //to reach the summary histograms of this detector.
  Bool_t found = kFALSE;
  TResults *pglob;
  TString s;
  TStatVolD *p;
  TStatVolD *pvol=0;
  pglob = GlobalP();
  TIter next(&pglob->fVolDet);
  while ((!found) && (p = (TStatVolD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      pvol = p;
    } 
  }
  return pvol;
}
TResults* TPublication::GlobalP() const {
  //  Finds the pointer to the global statistics
  TResults *p;
  TResults *pglob = 0;
  TIter next(&gLit->fResults);
  Bool_t found = kFALSE;
  while ((!found) && (p = (TResults *)next())) {
    if (!p->fRun) {
      found = kTRUE;
      pglob = p;
    }
  }
  if (!found) {
    if (gGs) pglob = gGs;
    else {
      pglob = 0;
      gMes->SetName("GlobalP");
      gMes->SetTitle("TPublication");
      gMes->M(Error_M,1,"Global statistics not found");
    }
  }
  return pglob;
}
void TPublication::InitP() {
  fHEfficiency = 0;
  fHAbsMat     = 0;
  fHAbsBef     = 0;
  fHAbsRvt     = 0;
  fHOutSide    = 0;
  fHAbnorm     = 0;
  fHTooLate    = 0;
  fHTooFew     = 0;
  fHLossAng    = 0;
  fHLossQE     = 0;
}
void TPublication::N() {
  //  Draw each histogram in turn when the user presses 'n'. Exit for any other
  //key hit.
  char c='n';
  gTwoPad->SelectPad1();
  while (c == 'n' ) {
    N1();
    cout << "Press n to see next histo, any other key to quit" << endl;
    cin >> c;
  }
}
void TPublication::N1() {
  // Draws histogram fK
  const Int_t nbasehist = 10;
  Int_t ddt,kdet,kh;
  if (!gTwoPad) gLit->BookCanvas();
  gTwoPad->ChangePad();
  if (gLit) gLit->RestoreComments();
  if (fK>=nbasehist) {
    ddt  = fK - nbasehist;
    kdet  = ddt/fNHperDet;
    kh   = ddt % fNHperDet;
    D(kdet,kh);
  }
  else {
    switch (fK) {
    case 0:
      DrawEfficiency();
      break;
    case 1:
      DrawAbsMat();
      break;
    case 2:
      DrawAbsBef();
      break;
    case 3:
      DrawAbsRvt();
      break;
    case 4:
      DrawOutSide();
      break;
    case 5:
      DrawAbnorm();
      break;
    case 6:
      DrawTooLate();
      break;
    case 7:
      DrawTooFew();
      break;
    case 8:
      DrawLossAng();
      break;
    case 9:
      DrawLossQE();
      break;
    default:
      fK = 0;
      DrawEfficiency();
      break;
    }
  }
  fK += 1;
  fK %= nbasehist + fNDet*fNHperDet;
  gTwoPad->UpdateIt();
}
void TPublication::Summary() {
  //Calculates the histograms summary of all runs
  gGs->Conclusion();
  if (fInMem) {
    if (gLit->fNResults>=2) {
      gLit->fResults.Sort();
      BookHistos();
      FillHistos();
    }
    else {
      gMes->SetName("Summary");
      gMes->SetTitle("TPublication");
      gMes->M(Error_M,1,"No statistics available");
    }
  }
  else {
    gMes->SetName("Summary");
    gMes->SetTitle("TPublication");
    gMes->M(Error_M,2,"Statistics not in memory");
    gMes->M(Info_M,2,"Put runstat=kTRUE and ForgetLast=kFALSE in Gen");
  }
}
