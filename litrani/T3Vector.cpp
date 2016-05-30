// @(#)root/html:$Id: T3Vector.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TMessErr.h"
#include "T3Vector.h"

ClassImp(T3Vector)

//______________________________________________________________________________
//
// Implement real 3-vectors in Litrani, with many operations using vectors.
//Look at all the methods for documentation.
//
//______________________________________________________________________________

T3Vector::T3Vector(Double_t a) {
  //constructor with all elements initialized to same value
  fV0 = a;
  fV1 = a;
  fV2 = a;
}
T3Vector::T3Vector(Double_t v1,Double_t v2,Double_t v3) {
  //constructor with 3 components given
  fV0 = v1;
  fV1 = v2;
  fV2 = v3;
}
T3Vector::T3Vector(const T3Vector &v) {
  //constructor with vector given
  fV0 = v.fV0;
  fV1 = v.fV1;
  fV2 = v.fV2;
}
Bool_t T3Vector::AlfaDelta(Double_t stheta, Double_t ctheta,Double_t &salfa,
  Double_t &calfa, Double_t &sdelta, Double_t &cdelta) const {
  //From the unit T3Vector "this" calculates alfa and delta for the decomposition :
  //
  //       x[0] = sin(delta)*cos(alfa)
  //       x[1] = cos(delta)*cos(alfa)
  //       x[2] = sin(alfa)
  //
  //AlfaDelta return true, except in the case where cos(alfa) is 0. In that
  //case, delta is undefined, AlfaDelta returns false and arbitrarily puts
  //sin(delta) = sin(theta), cos(delta) = cos(theta).
  //
  //The problem is undefined in the sense that there are 2 possible solutions :
  //we can change the signs of calfa,sdelta and cdelta : we still have a solution.
  //So we choose the solution which is closest stheta and ctheta.
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  Double_t calf2,sdelt2,cdelt2;
  Double_t dd1,dd2;
  Bool_t b;
  Double_t r;
  salfa = fV2;
  calfa = TMath::Sqrt(un - salfa*salfa);
  r     = TMath::Abs(calfa);
  if (r>wsmall) {
    b      = kTRUE;
    sdelta = fV0/calfa;
    cdelta = fV1/calfa;
    calf2  = - calfa;
    sdelt2 = - sdelta;
    cdelt2 = - cdelta;
    dd1    = TMath::Abs(sdelta - stheta) + TMath::Abs(cdelta - ctheta);
    dd2    = TMath::Abs(sdelt2 - stheta) + TMath::Abs(cdelt2 - ctheta);
    if (dd2<dd1) {
      calfa  = calf2;
      sdelta = sdelt2;
      cdelta = cdelt2;
    }
  }
  else {
    b      = kFALSE;
    salfa  = un;
    calfa  = zero;
    sdelta = stheta;
    cdelta = ctheta;
  }
  return b;
}
Short_t T3Vector::Biggest() const {
  //Finds which of the 3 components fV0,fV1,fV2 is the biggest in absolute value
  Short_t k;
  Double_t a0,a1,a2;
  a0 = TMath::Abs(fV0);
  a1 = TMath::Abs(fV1);
  a2 = TMath::Abs(fV2);
  if (a0>=a1) {
    if (a0>=a2) {
      k = 0;
    }
    else {
      k = 2;
    }
  }
  else {
    if (a1>=a2) {
      k = 1;
    }
    else {
      k = 2;
    }
  }
  return k;
}
Double_t T3Vector::CrossCylinder(const T3Vector &nd, Int_t &nsolb, Int_t &nsoli,
  Double_t &t1, Double_t &t2, T3Vector &c1, T3Vector &c2, const T3Vector &cc,
  const T3Vector &a, Double_t r, Double_t h, Double_t &dist1, Double_t &dist2,
  Double_t tcut) const {
  //  Calculates the intersection of a straight line with a cylinder.  The
  //equation of the straight line is : *this + t*nd . Only intersections which
  //are located AFTER the point *this + tcut*nd, i.e. for which t>tcut, are
  //looked for. If an intersection with t1/2<=tcut is found, t1/2 has the right
  //value of the solution, sol1/2 is the right value  *this + t1/2*nd of the
  //intersection of the straigth line with the infinite cylinder containing
  //the bounded cylinder, BUT : nsolb only indicates the number of solutions
  //for which the bounded cylinder is hit and t>tcut.
  //
  //  PARAMETERS
  //
  //  this  : the straight line passes by the point represented by *this.
  //  nd    : the straight line has a direction given by the unit vector nd.
  //  nsolb : number of solution 0, 1 or 2. This is the number of solutions
  //          for the intersection with the bounded cylinder, not the infinite
  //          cylinder containing the bounded cylinder.
  //  nsoli : number of solution 0, 1 or 2. This is the number of solutions
  //          for the intersection with the infinite cylinder containing the
  //          bounded cylinder.
  //  t1,t2 : t is the parameter which moves the point on the straight line.
  //          On return, t1 contains the t value for 1st intersection, t2 the
  //          t value for 2nd intersection. t1, t2 are the values for the
  //          intersection with the infinite cylinder. Look at nsolb to know
  //          how many of these values are valid for the bounded cylinder.
  //          If nsoli=2 and nsolb = 1, t1 is the solution also valid for the
  //          bounded cylinder.
  //  c1,c2 : the 2 solutions. If there are 2 good solutions ( which is
  //          testified by the values returned in dist and dist1/2 see discus-
  //          sion on return value below ), c1 and c2 are identical to the
  //          solutions pointed to by t1 and t2. This means that :
  //               c1 = *this + t1*nd      and      c2 = *this + t2*nd
  //          If there are no solution, c1 is the point on the cylinder which
  //          is closest to the straight line.
  //  cc    : center of the "lower" basis of the cylinder
  //  a     : unit vector, axis of the cylinder pointing from cc towards
  //          "upper" basis center.
  //  r     : radius of the cylinder
  //  h     : height of the cylinder
  //  dist1 : negative distance of 1st solution from the closest edge (circle)
  //          of the cylinder. If it is slightly positiv, it means that the
  //          found solution was slightly off the bounded cylinder by dist1.
  //          It is up to the user to accept or not this slightly bad solution.
  //  dist2 : negative distance of 2nd solution from the closest edge (circle)
  //          of the cylinder. If it is slightly positiv, it means that the
  //          found solution was slightly off the bounded cylinder by dist2.
  //          It is up to the user to accept or not this slightly bad solution.
  //  tcut  : value of t below which a solution is NOT accepted. This
  //          parameter is useful for instance in the case where one does
  //          not want to find intersections which are behind the current
  //          point *this. In that case, put tcut=0 !
  //
  //  The returned value desserves a detailed explanation :
  //
  //  The returned value is an indication of how well the straight line inter-
  //cepts the INFINITE cylinder.
  //
  // (1) - If this returned value is negative, it means that the intersection
  //       is ok. nsoli will then be equal to 2.
  // (2) - If it is slightly positiv and nsoli=1, it means that the straight
  //       line passes very close to the INFINITE cylinder. We still have
  //       accepted this slightly bad solution, in order to cope with nume-
  //       rical imprecision. The returned value is then the distance between
  //       the straight line and the INFINITE cylinder. The user is free, if
  //       he wishes so, to refuse this solution, or to make more severe cuts
  //       on this returned value.
  // (3) - If it is positive and nsoli=0, it means that the straight line does
  //       not intercepts the INFINITE cylinder.
  //
  //  Notice that all this concerns the intercept between the straight line
  //and the INFINITE cylinder. For the intercept between the straight line and
  //the BOUNDED cylinder, you get the information about how close solution 1
  //and 2 are from the edges (circles) of the BOUNDED cylinder by the returned
  //parameters dist1 and dist2.
  //
  //  (1)   (a) - If dist1 is negativ and nsolb >=1, the 1st intersection is
  //       inside the BOUNDED cylinder. -dist1 is the distance from the 1st
  //       solution to the nearest edge (circle) of the BOUNDED cylinder. We
  //       have then: c1 = *this + t1*nd.
  //        (b) - If dist1 is positiv and nsolb = 0, the 1st solution is not
  //       on the BOUNDED cylinder.
  //        (c) - If dist1 is slightly positive and nsolb >=1, it means that
  //       the 1st solution is very close to be on the BOUNDED cylinder. We
  //       still have accepted this slightly bad solution, in order to cope
  //       with numerical imprecision. dist1 is then the distance between the
  //       slightly out 1st solution and the BOUNDED cylinder. The user is
  //       free, if he wishes so, to refuse this solution, or to make more
  //       severe cuts on dist1. In case (c), c1 != *this + t1*nd, because
  //       c1 has been pushed onto the bounded cylinder. c1 is on the bounded
  //       cylinder, *this + t1*nd is not.
  //  (2)   (a) - If dist2 is negativ and nsolb ==2, the 2nd intersection is
  //       inside the BOUNDED cylinder. -dist2 is the distance from the 2nd
  //       solution to the nearest edge (circle) of the BOUNDED cylinder. We
  //       have then: c2 = *this + t2*nd.
  //        (b) - If dist2 is positiv and nsolb <= 1, the 2nd solution is not
  //       on the BOUNDED cylinder.
  //        (c) - If dist2 is slightly positive and nsolb ==2, it means that
  //       the 2nd solution is very close to be on the BOUNDED cylinder. We
  //       still have accepted this slightly bad solution, in order to cope
  //       with numerical imprecision. dist2 is then the distance between the
  //       slightly out 2nd solution and the BOUNDED cylinder. The user is
  //       free, if he wishes so, to refuse this solution, or to make more
  //       severe cuts on dist2. In case (c), c2 != *this + t2*nd, because
  //       c2 has been pushed onto the bounded cylinder. c2 is on the bounded
  //       cylinder, *this + t2*nd is not.
  //
  //  Notice that in the case where nsolb = 2, we return as 1st solution the
  //one which is FARTHEST from *this, and as second solution, the one which is
  //CLOSEST from *this. This may seem strange, but corresponds well to the
  //situation where one is already on the cylinder, and one looks for the
  //other intercept, which is the normal case in Litrani.
  //
  enum Quality { bad, limit, good };
  const Double_t un      = 1.0;
  const Double_t zero    = 0.0;
  const Double_t dixm6   = 1.0E-6;
  const Double_t dixm300 = 1.0E-300;
  const Double_t dix300  = 1.0E+300;
  Double_t epsilon;
  Double_t clength;
  Quality q0;
  Quality q1=bad, q2=bad;
  Double_t nb, na, ab;  //scalar products
  Double_t dist=zero;   //distance from straight line to cylinder
  //                        == distance from straight line to axis of cylinder - r
  Double_t AA,BB,CC,DD; //2nd order equation
  Double_t h1,h2;       //height of the 2 solutions
  Double_t td, ta;      //move on straight line and on cylinder axis
  T3Vector xp(0);       //point on straight line closest to cylinder
  T3Vector vp(0);       //point on cylinder axis closest to straight line
  T3Vector up(0);       //unit vector from vp to xp
  T3Vector b(0);        //cc - *this
  T3Vector x3(dix300);  //from c1 to c2
  clength = (r+h)/2.0;
  epsilon = dixm6*clength;
  //closest distance between straight line and cylinder
  b  = cc - *this;
  nb = nd*b;
  na = nd*a;
  ab = a*b;
  AA = un - na*na;
  DD = TMath::Abs(AA);
  if (DD<dixm300) {
    nsoli = 0;
    dist1   = un;
    dist2   = un;
    c1    = x3;
    c2    = c1;
  }
  else {
    td = (nb - na*ab)/AA;
    ta = (nb*na - ab)/AA;
    xp = *this + td*nd;
    vp = cc    + ta*a;
    up = xp - vp;
    dist  = up.Module();
    dist -= r;
    up.Unit();
    c1 = vp + r*up;
    c2 = c1;
    // Intersection with the cylinder. If x is a point on the cylinder and if xa
    //is the nearest point (from x) on the axis of the cylinder, we must have
    //(x-xa)*a = 0. If one writes xa = cc + v*a, it results v = (x-cc)*a. So the
    //equation of the cylinder is :
    // r^2 = (x-xa)^2 . But x must also be on the straight line, hence
    // x = *this + t*nd. The equation for x on the straight line and on the
    //cylinder is :
    //  r^2 = (*this + t*nd - cc - v*a)^2
    //  r^2 = ( t*nd - b - [(x-cc)*a]*a )^2
    BB = -td;
    CC = (b.Norm2Sqr() - ab*ab - r*r)/AA;
    DD = BB*BB - CC;
    if ((DD<=zero) && (dist<epsilon)) {
      nsoli = 1;
      DD    = zero;
      t1    = -BB;
      t2    = -BB;
      c1    = *this + t1*nd;
      c2    = c1;
    }
    else {
      if (DD>zero) {
        nsoli = 2;
        DD    = TMath::Sqrt(DD);
        t1    = -BB + DD;
        t2    = -BB - DD;
        c1    = *this + t1*nd;
        c2    = *this + t2*nd;
      }
      else nsoli = 0;
    }
  }
  //are the solutions inside the bounded cylinder ?
  nsolb = 0;
  if (nsoli) {
    x3 = c1 - cc;
    h1 = a*x3;
    dist1 = TMath::Max(-h1, h1 - h);
    if (dist1<epsilon) {
      nsolb += 1;
      if (dist1<=zero) q1 = good;
      else {
        q1 = limit;
        //it is necessary to push the point back towards the bounded cylinder
        if (h1<zero) c1 = c1 - h1*a;
        if (h1>h)    c1 = c1 - (h1-h)*a;
      }
    }
  }
  if (nsoli==2) {
    x3 = c2 - cc;
    h2 = a*x3;
    dist2 = TMath::Max(-h2, h2-h);
    if (dist2<epsilon)  {
      nsolb += 1;
      if (dist2<=zero) q2 = good;
      else {
        q2 = limit;
        //it is necessary to push the point back towards the bounded cylinder
        if (h2<zero) c2 = c2 - h2*a;
        if (h2>h)    c2 = c2 - (h2-h)*a;
      }
    }
  }
  //decides of the order in which solutions are presented
  Bool_t exchange = kFALSE;
  if ((nsolb==1) && (q1==bad)) exchange = kTRUE;
  if (nsolb==2) {
    switch (q1) {
      case bad:
        break;
      case limit:
        switch (q2) {
      case bad:
        break;
      case limit:
      case good:
        if (t2>t1) exchange = kTRUE;
        break;
        }
        break;
      case good:
        switch (q2) {
      case bad:
        break;
      case limit:
      case good:
        if (t2>t1) exchange = kTRUE;
        break;
        }
        break;
    }
  }
  if (exchange) {
    AA    = t1;
    q0    = q1;
    BB    = dist1;
    x3    = c1;
    t1    = t2;
    q1    = q2;
    dist1 = dist2;
    c1    = c2;
    t2    = AA;
    q2    = q0;
    dist2 = BB;
    c2    = x3;
  }
  //do not accept solutions with negative or 0 t values
  if ((q2!=bad) && (t2<=tcut)) nsolb -= 1;
  if ((q1!=bad) && (t1<=tcut)) nsolb -= 1;
  return dist;
}
Double_t T3Vector::CrossDisk(const T3Vector &nd, Double_t &t, T3Vector &sol1,Int_t &nsol,
  const T3Vector &nc, const T3Vector &cc, Double_t radius, Double_t tcut) const {
  //Calculates the intersection of a straight line with a disk. The equa-
  //tion of the straight line is : *this + t*nd . Only intersections which are
  //located AFTER the point *this + tcut*nd, i.e. for which t>tcut, are looked
  //for. If an intersection with t<=tcut is found, t has the right value of
  //the solution, sol1 is the right value  *this + t*nd of the intersection
  //of the straigth line with the infinite plane containing the disk, BUT :
  //nsol is returned 0 and the returned value is big and positiv. No check has
  //been done that the intersection was inside the disk !
  //
  //  this   : the straight line passes by the point represented by "this".
  //  nd     : the straight line has a direction given by the unit vector nd.
  //  t      : the parameter which moves the point on the straight line is t.
  //           On return, t contains the value which is the intersection point
  //           with the infinite plane containing the disk.
  //  sol1   : Let us call dist the return value of the method CrossDisk.
  //             (a) - If ((dist <= 0) && (nsol == 1)) there is a good solu-
  //                 tion inside the disk.
  //                 In that case, sol1 = *this + t*nd
  //             (b) - If ((dist >  0) && (nsol == 1)) and dist is small,
  //                 there is almost a good solution. The intersection was
  //                 outside the disk by the small distance dist. We still
  //                 take the solution as good to cope with numerical impre-
  //                 cision, but the user is free either to reject this solu-
  //                 tion or to make more severe cuts on dist.
  //                 In that case : sol1 != x0 + t*nd !!! 
  //                   -  x0 + t*nd is the true intersection, slightly outside
  //                                the disk,
  //                   -  sol1 has been pushed inside the disk.
  //             (c) - If ((dist >  0) && (nsol == 0)) there is no inter-
  //                 section inside the disk.
  //  nsol   : number of solutions : 0 or 1.
  //  nc     : normal to the plane containing the disk.
  //  cc     : center of the disk
  //  radius : radius of the disk
  //  tcut   : value of t below which a solution is NOT accepted. This
  //           parameter is useful for instance in the case where one does
  //           not want to find intersections which are behind the current
  //           point *this. In that case, put tcut=0 !
  //
  //  If the returned value is negativ, it means that the intersection of the
  //straight line with the disk is inside the disk. If it is positiv, it is
  //outside. This returned value is the closest distance of the straight line
  //to the disk. If it is positiv, but small (< ~1.0e-4), it can be that the
  //point was indeed inside, but the return value is positiv due to numerical
  //imprecision. In that case, use "sol1" instead of *this + t*nd as the
  //intersection.  "*this + t*nd" is the true value of the intersection of the
  //straight line with the infinite plane containing the disk. "sol1" has
  //been pushed inside the disk.
  //
  //  BE CAREFUL THAT nsol = 1 DOES NOT MEAN THERE IS 1 GOOD SOLUTION !
  //  You are free to choose for yourself any limit smaller than 1.0E-4 to
  //decide for a cut between accepted and not accepted solution.
  //
  const Double_t zero    = 0.0;
  const Double_t dixm5   = 1.0E-5;
  const Double_t epsilon = 1.0E-9;
  const Double_t dixp300 = 1.0E+300;
  const Double_t dixm300 = 1.0E-300;
  Double_t toomuch;
  T3Vector w(0);          //for intermediate calculations
  T3Vector vp(dixp300);   //intersection with the plane containing the disk
  Double_t dist;          //distance of vp to disk
  Double_t d1,d2,ad2;
  toomuch = dixm5*radius; // limit behond which point irremediably conside-
  //red outside
  //Intersection with the plane containing the disk ---> vp
  nsol = 1;
  w   = cc - *this;
  d1  = nc*w;
  d2  = nc*nd;
  ad2 = TMath::Abs(d2);
  if (ad2<dixm300) {
    nsol = 0;
    t = dixp300;
    sol1 = vp;
    dist = TMath::Abs(d1);
  }
  else {
    t  = d1/d2;
    vp = *this + t*nd;
    sol1 = vp;
    //If t<=tcut, this solution is rejected
    if (t<=tcut) {
      nsol   = 0;
      dist   = dixp300;
    }
    else {
      //Is vp inside disk ?
      w  = vp - cc;
      dist = w.Module();
      w.Unit();
      dist -= radius;
      if (dist > zero) {
        if (dist<=toomuch) sol1 = vp - (dist+epsilon)*w;
        else nsol = 0;
      }
    }
  }
  return dist;
}
Double_t T3Vector::CrossPolygon(const T3Vector &nd, Double_t &t, T3Vector &sol1,
  Int_t &nsol,Int_t nvertex, const T3Vector &np, const TObjArray &pts,
  const TObjArray &outridge, Double_t tcut, Double_t clength) const {
  //
  //  Calculates the intersection of a straight line with a polygon. The equa-
  //tion of the straight line is : *this + t*nd . Only intersections which are
  //located AFTER the point *this + tcut*nd, i.e. for which t>tcut, are looked
  //for. If an intersection with t<=tcut is found, t has the right value of
  //the solution, sol1 is the right value  *this + t*nd of the intersection
  //of the straigth line with the infinite plane containing the polygon, BUT :
  //nsol is returned 0 and the returned value is big and positiv. No check has
  //been done that the intersection was inside the polygon !
  //
  //  Parameters :
  //
  //  this     : the straight line passes by the point represented by "this".
  //  nd       : the straight line has a direction given by the unit vector nd.
  //  t        : the parameter which moves the point on the straight line is t.
  //             On return, t contains the value which is the intersection
  //             point with the infinite plane containing the polygon.
  //  sol1     : Let us call dist the return value of the method CrossPolygon.
  //             (a) - If ((dist <= 0) && (nsol == 1)) there is a good solu-
  //                 tion inside the polygon.
  //                 In that case, sol1 = *this + t*nd
  //             (b) - If ((dist >  0) && (nsol == 1)) and dist is small,
  //                 there is almost a good solution. The intersection was
  //                 outside the polygon by the small distance dist. We still
  //                 take the solution as good to cope with numerical impre-
  //                 cision, but the user is free either to reject this solu-
  //                 tion or to make more severe cuts on dist.
  //                 In that case : sol1 != x0 + t*nd !!! 
  //                   -  x0 + t*nd is the true intersection, slightly outside
  //                                the polygon,
  //                   -  sol1 has been pushed inside the polygon.
  //             (c) - If ((dist >  0) && (nsol == 0)) there is no inter-
  //                 section inside the polygon.
  //  nsol     : number of solutions : 0 or 1.
  //  nvertex  : is the number of vertices of the polygon
  //  np       : is the normal to the plane containing the polygon
  //  pts      : is the list of vertices of the polygon
  //  outridge : is the list of all vectors attached to the vertices, in the
  //             plane of the polygon, orthogonal to the vector going from
  //             point i to point i+1 ( if attached to point i ), and pointing
  //             outside the polygon.
  //  tcut     : value of t below which a solution is NOT accepted. This
  //             parameter is useful for instance in the case where one does
  //             not want to find intersections which are behind the current
  //             point *this. In that case, put tcut=0 !
  //  clength  : clength is a characteristic length for the polygon. Take it
  //             of the order of the square root of the surface of the polygon.
  //             If you do not give it, it will be taken as 1. This charac-
  //             teristic length is used to set the cuts that decide whether a
  //             solution is "close" to the surface or not. It is clear that
  //             if your surface is for instance very tiny, these cuts have to
  //             be very small. On the contrary, they are to be taken much
  //             broader if the surface is extremely large. If your surface
  //             has natural dimensions of the order of cm you can omit this
  //             parameter.
  //
  //  If the returned value is negativ, it means that the intersection of the
  //straight line with the polygon is inside the polygon. If it is positiv, it
  //is outside. This returned value is the closest distance of the straight
  //line to the polygon. If it is positiv, but small, it can be
  //that the point was indeed inside, but the return value is positiv due to
  //numerical imprecision. Suppose for instance that "this" is on one face of
  //a convex polyhedron and you look which is the other face of the convex
  //polyhedron which is hit by the straight line. As the polyhedron is convex,
  //you are absolutly sure that an other face is indeed hit. But in the loop
  //on all faces, calling CrossPolyhedron, you only return positiv values.
  //Take for granted that it is due to numerical imprecision and that one of
  //the return values at least was very small and indicates the face which was
  //hit. In that case, use "sol1" instead of "*this + t*nd" as intersection.
  //  "*this + t*nd" is the true value of the intersection of the straight
  //line with the infinite plane containing the polygon. "sol1" has been
  //pushed inside the polygon.
  //
  //  BE CAREFUL THAT nsol = 1 DOES NOT MEAN THERE IS 1 GOOD SOLUTION !
  //
  const Double_t zero    = 0.0;
  const Double_t dixm5   = 1.0E-5;
  const Double_t epsilon = 1.0E-9;
  const Double_t dixp300 = 1.0E+300;
  const Double_t dixm300 = 1.0E-300;
  Double_t toomuch;
  Double_t dist;          //distance of vp to polygon
  T3Vector *pi;           //points of the polygon
  T3Vector w(0);          //for intermediate calculations
  T3Vector vp(dixp300);   //intersection with the plane containing the polygon
  T3Vector *ns;           //outridge vector
  T3Vector nskeep(0);     //keep problematic *ns
  Double_t d1,d2,ad2;
  toomuch = dixm5*clength;//limit behond which point irremediably conside-
  //                          red outside
  //Intersection with the plane containing the polygon ---> vp
  nsol = 1;
  pi  = (T3Vector *)pts[0];
  w   = *pi - *this;
  d1  = np*w;
  d2  = np*nd;
  ad2 = TMath::Abs(d2);
  if (ad2<dixm300) {
    nsol = 0;
    t    = dixp300;
    sol1 = vp;
    dist = TMath::Abs(d1);
  }
  else {
    t  = d1/d2;
    vp = *this + t*nd;
    //If t<=tcut, this solution is rejected
    if (t<=tcut) {
      nsol = 0;
      dist = dixp300;
      sol1 = vp;
    }
    else {
      //Is vp inside polygon ?
      Bool_t dedans = kTRUE;
      Int_t i = 0;
      dist = -dixp300;
      while ((dedans) && (i<nvertex)) {
        ns = (T3Vector *)outridge[i];
        pi = (T3Vector *)pts[i];
        w  = vp - *pi;
        d1 = (*ns)*w;
        if (d1>dist) {
          dist = d1;
          nskeep = *ns;
        }
        if (d1>toomuch) {
          dedans = kFALSE;
          nsol   = 0;
          sol1   = vp;
        }
        i++;
      }//end while
      if (dedans) {
        if (dist<=zero) sol1 = vp;
        else sol1 = vp - (dist+epsilon)*nskeep;  //push point inside
      }//end if (dedans)
    }//end else if (t<=zero)
  }//end else if (ad2<dixm300)
  return dist;
}
Double_t T3Vector::CrossRing(const T3Vector &nd, Double_t &t, T3Vector &sol1,Int_t &nsol,
  const T3Vector &nc, const T3Vector &cc, Double_t BigR,Double_t SmallR, Double_t tcut) const {
  //  Calculates the intersection of a straight line with a ring. The equation
  //of the straight line is : *this + t*nd . Only intersections which are
  //located AFTER the point *this + tcut*nd, i.e. for which t>tcut, are looked
  //for. If an intersection with t<=tcut is found, t has the right value of
  //the solution, sol1 is the right value  *this + t*nd of the intersection
  //of the straigth line with the infinite plane containing the ring, BUT :
  //nsol is returned 0 and the returned value is big and positiv. No check has
  //been done that the intersection was inside the ring !
  //
  //  this   : the straight line passes by the point represented by "this".
  //  nd     : the straight line has a direction given by the unit vector nd.
  //  t      : the parameter which moves the point on the straight line is t.
  //           On return, t contains the value which is the intersection point
  //           with the plane containing the ring.
  //  sol1   : Let us call dist the return value of the method CrossRing.
  //             (a) - If ((dist <= 0) && (nsol == 1)) there is a good solu-
  //                 tion inside the ring.
  //                 In that case, sol1 = *this + t*nd
  //             (b) - If ((dist >  0) && (nsol == 1)) and dist is small,
  //                 there is almost a good solution. The intersection was
  //                 outside the ring by the small distance dist. We still
  //                 take the solution as good to cope with numerical impre-
  //                 cision, but the user is free either to reject this solu-
  //                 tion or to make more severe cuts on dist.
  //                 In that case : sol1 != x0 + t*nd !!! 
  //                   -  x0 + t*nd is the true intersection, slightly outside
  //                                the ring,
  //                   -  sol1 has been pushed inside the ring.
  //             (c) - If ((dist >  0) && (nsol == 0)) there is no inter-
  //                 section inside the ring.
  //  nsol   : number of solutions : 0 or 1.
  //  nc     : normal to the plane containing the ring.
  //  cc     : center of the ring
  //  BigR   : outside radius of the ring
  //  SmallR : inside radius of the ring
  //  tcut   : value of t below which a solution is NOT accepted. This
  //           parameter is useful for instance in the case where one does
  //           not want to find intersections which are behind the current
  //           point *this. In that case, put tcut=0 !
  //
  //  If the returned value is negativ, it means that the intersection of the
  //straight line with the ring is inside the ring. If it is positiv, it is
  //outside. This returned value is the closest distance of the straight line
  //to the ring. If it is positiv, but small (< ~1.0e-4), it can be that the
  //point was indeed inside, but the return value is positiv due to numerical
  //imprecision. In that case, use "sol1" instead of *this + t*nd as the
  //solution.  "*this + t*nd" is the true value of the intersection of the
  //straight line with the infinite plane containing the ring. "sol1" has
  //been pushed inside the ring.
  //
  //  BE CAREFUL THAT nsol = 1 DOES NOT MEAN THERE IS 1 GOOD SOLUTION !
  //
  const Double_t zero    = 0.0;
  const Double_t dixm5   = 1.0E-5;  // limit behond which point irremediably considered outside
  const Double_t epsilon = 1.0E-9;
  const Double_t dixp300 = 1.0E+300;
  const Double_t dixm300 = 1.0E-300;
  Double_t toomuch;
  T3Vector w(0);          //for intermediate calculations
  T3Vector vp(dixp300);   //intersection with the plane containing the ring
  Double_t dist;          //distance of vp to ring
  Double_t d0,d1,d2,ad2;
  toomuch = dixm5*BigR;
  Bool_t dedans = kTRUE;
  //Intersection with the plane containing the ring ---> vp
  nsol = 1;
  w = cc - *this;
  d1 = nc*w;
  d2 = nc*nd;
  ad2 = TMath::Abs(d2);
  if (ad2<dixm300) {
    nsol = 0;
    t = dixp300;
    sol1 = vp;
    dist = TMath::Abs(d1);
  }
  else {
    t    = d1/d2;
    vp   = *this + t*nd;
    sol1 = vp;
    //If t<=tcut, this solution is rejected
    if (t<=tcut) {
      nsol   = 0;
      dist   = dixp300;
    }
    else {
      //Is vp inside outer ring ?
      w    = vp - cc;
      d0   = w.Module();
      w.Unit();
      dist = d0 - BigR;
      if (dist>zero) {
        if (dist<=toomuch) sol1 = vp - (dist+epsilon)*w;
        else {
          dedans = kFALSE;
          nsol = 0;
        }
      }
      else {
        //Is vp outside inner ring ?
        d2 = SmallR - d0;
        if (d2>zero) {
          dist = d2;
          if (d2<=toomuch) sol1 = vp + (d2+epsilon)*w;
          else {
            dedans = kFALSE;
            nsol   = 0;
          }
        }
        else if (d2>dist) dist = d2;
      }
    }
  }
  return dist;
}
Double_t T3Vector::CrossTCone(const T3Vector &nd, Int_t &nsolb, Int_t &nsoli,
  Double_t &t1, Double_t &t2, T3Vector &c1, T3Vector &c2, const T3Vector &cc,
  const T3Vector &a, Double_t r, Double_t h,Double_t H,Double_t &dist1,
  Double_t &dist2, Double_t tcut) const {
  //  Calculates the intersection of a straight line with a truncated cone.
  //
  //  PARAMETERS
  //
  //  this  : the straight line passes by the point represented by *this.
  //  nd    : the straight line has a direction given by the unit vector nd.
  //  nsolb : number of solution 0, 1 or 2. This is the number of solutions
  //          for the intersection with the truncated cone, not the infinite
  //          cone containing the truncated cone.
  //  nsoli : number of solution 0, 1 or 2. This is the number of solutions
  //          for the intersection with the infinite cone containing the
  //          truncated cone.
  //  t1,t2 : t is the parameter which moves the point on the straight line.
  //          On return, t1 contains t value for 1st intersection, t2 t value
  //          for 2nd intersection. t1, t2 are the values for the intersection
  //          with the infinite cone. Look at nsolb to know how many of these
  //          values are valid for the truncated cone.
  //          if nsoli=2 and nsolb = 1, t1 is the solution also valid for the
  //          truncated cone.
  //  c1,c2 : the 2 solutions. If there are 2 good solutions ( which is
  //          testified by the values returned in dist and dist1/2 see discus-
  //          sion on return value below ), c1 and c2 are identical to the
  //          solutions pointed to by t1 and t2. This means that :
  //               c1 = *this + t1*nd      and      c2 = *this + t2*nd
  //  cc    : center of the large basis of the truncated cone.
  //  a     : unit vector, axis of the truncated cone pointing from cc
  //          towards top of cone.
  //  r     : radius of the large basis of the truncated cone.
  //  h     : height of the truncated cone.
  //  H     : height from cc to top of cone.
  //  dist1 : negative distance of 1st solution from the closest edge (circle)
  //          of the cylinder. If it is slightly positiv, it means that the
  //          found solution was slightly off the bounded cylinder by dist1.
  //          It is up to the user to accept or not this slightly bad solution.
  //  dist2 : negative distance of 2nd solution from the closest edge (circle)
  //          of the cylinder. If it is slightly positiv, it means that the
  //          found solution was slightly off the bounded cylinder by dist2.
  //          It is up to the user to accept or not this slightly bad solution.
  //  tcut  : value of t below which a solution is NOT accepted. This
  //          parameter is useful for instance in the case where one does
  //          not want to find intersections which are behind the current
  //          point *this. In that case, put tcut=0 !
  //
  //  The returned value desserves a detailed explanation :
  //
  //  The returned value is an indication of how well the straight line inter-
  //cepts the INFINITE cone.
  //
  // (1) - If this returned value is negative, it means that the intersection
  //       is ok. nsoli will then be equal to 2.
  // (2) - If it is slightly positiv and nsoli=1, it means that the straight
  //       line passes very close to the INFINITE cone. We still have
  //       accepted this slightly bad solution, in order to cope with nume-
  //       rical imprecision. The returned value is then the distance between
  //       the straight line and the INFINITE cone. The user is free, if
  //       he wishes so, to refuse this solution, or to make more severe cuts
  //       on this returned value.
  // (3) - If it is positive and nsoli=0, it means that the straight line does
  //       not intercepts the INFINITE cone.
  //
  //  Notice that all this concerns the intercept between the straight line
  //and the INFINITE cone. For the intercept between the straight line and
  //the BOUNDED cone, you get the information about how close solution 1
  //and 2 are from the edges (circles) of the BOUNDED cone by the returned
  //parameters dist1 and dist2.
  //
  //  (1)   (a) - If dist1 is negativ and nsolb >=1, the 1st intersection is
  //       inside the BOUNDED cone. -dist1 is the distance from the 1st solu-
  //       tion to the nearest edge (circle) of the BOUNDED cone. We have
  //       then:  c1 = *this + t1*nd.
  //        (b) - If dist1 is positiv and nsolb = 0, the 1st solution is not
  //       on the BOUNDED cone.
  //        (c) - If dist1 is slightly positive and nsolb >=1, it means that
  //       the 1st solution is very close to be on the BOUNDED cone. We
  //       still have accepted this slightly bad solution, in order to cope
  //       with numerical imprecision. dist1 is then the distance between the
  //       slightly out 1st solution and the BOUNDED cone. The user is
  //       free, if he wishes so, to refuse this solution, or to make more
  //       severe cuts on dist1. In case (c), c1 != *this + t1*nd, because
  //       c1 has been pushed onto the bounded cone. c1 is on the bounded
  //       cone, *this + t1*nd is not.
  //  (2)   (a) - If dist2 is negativ and nsolb ==2, the 2nd intersection is
  //       inside the bounded cone. -dist2 is the distance from the 2nd solu-
  //       tion to the nearest edge (circle) of the BOUNDED cone. We have
  //       then:  c2 = *this + t2*nd.
  //        (b) - If dist2 is positiv and nsolb <= 1, the 2nd solution is not
  //       on the BOUNDED cone.
  //        (c) - If dist2 is slightly positive and nsolb =2, it means that
  //       the 2nd solution is very close to be on the BOUNDED cone. We
  //       still have accepted this slightly bad solution, in order to cope
  //       with numerical imprecision. dist2 is then the distance between the
  //       slightly out 2nd solution and the BOUNDED cone. The user is
  //       free, if he wishes so, to refuse this solution, or to make more
  //       severe cuts on dist2. In case (c), c2 != *this + t2*nd, because
  //       c2 has been pushed onto the bounded cone. c2 is on the bounded
  //       cone, *this + t2*nd is not.
  //
  //  Notice that in the case where nsolb = 2, we return as 1st solution the
  //one which is FARTHEST from *this, and as second solution, the one which is
  //CLOSEST from *this. This may seem strange, but corresponds well to the
  //situation where one is already on the cone, and one looks for the
  //other intercept, which is the normal case in Litrani.
  //
  enum Quality { bad, limit, good };
  const Double_t zero    = 0.0;
  const Double_t un      = 1.0;
  const Double_t deux    = 2.0;
  const Double_t dixm300 = 1.0E-300;
  const Double_t dixm6   = 1.0E-6;
  const Double_t dixm10  = 1.0E-10;
  Quality q0;
  Quality q1=bad, q2=bad;
  Double_t dist;           //~distance from straight line to infinite cone
  Double_t nz;             //nz  = a*nd
  Double_t den;            //den = 1 - nz^2
  Double_t AA,BB,CC,DD;    //for 2nd order equation
  T3Vector e1(0),e2(0);    //coordinate system with z axis == a
  T3Vector s(0);           //top of the cone
  T3Vector b(0);           // = *this - s
  T3Vector x3(0);          //for calculation of height of point on
  Double_t tz;             //for intermediate calculation
  Double_t ta;             //tan(alpha) where alpha = opening angle of cone
  Double_t bx,by,ba;       // = (b,e1) , (b,e2) , (b,a)
  Double_t ny;             // (nd,e2)
  Double_t h1,h2;          //height of points c1 and c2
  Bool_t exchange = kFALSE;
  Double_t epsilon;
  Double_t clength;
  clength = (r+h)/2.0;
  epsilon = dixm6*clength;
  //We construct a coordinate system e1,e2,a. We take as e1 the axis which is
  //both orthogonal to a AND orthogonal to nd.
  nz = a*nd;
  s  = cc + H*a;
  b  = *this - s;
  ba = b*a;
  e1 = nd^a;
  tz = e1*e1;
  if (tz<dixm10) {
    //nd and a are parallel! This will end with e1 totally wrong when
    //calling e1.Unit(), because of division by almost 0. To avoid
    //this, we define e1 as nd^b. e1 will any way be orthogonal to
    //a because a is parallel to nd!
    e1 = nd^b;
  }
  e1.Unit();
  e2 = a^e1;
  e2.Unit();
  if (gMes->TestCalc()) {
    gMes->SetName("CrossTCone");
    gMes->SetTitle("T3Vector");
    den = (e1^e2)*a;
    if (TMath::Abs(un-den)>dixm6) 
      gMes->M(Error_M,1,"system non orthonormal");
  }
  //Intersection with infinite cone
  ta = r/H;
  bx = b*e1;
  by = b*e2;
  ny = nd*e2;
  DD = ta*ta;
  AA = ny*ny - DD*nz*nz;
  BB = by*ny - DD*ba*nz;
  CC = bx*bx + by*by - DD*ba*ba;
  DD = TMath::Abs(AA);
  if (DD<dixm300) {
    //here 1 or 0 solution
    if (TMath::Abs(BB)<dixm300) {
      nsoli = 0;
      dist  = un;
    }
    else {
      nsoli = 1;
      dist  = -un;
      t1    = - CC/(deux*BB);
      t2    = t1;
      c1    = *this + t1*nd;
      c2    = c1;
    }
  }
  else {
    //here 2 or 0 solutions
    BB = BB/AA;
    CC = CC/AA;
    DD = BB*BB - CC;
    if (DD>=zero) {
      //here there are 2 intersections with the infinite cone
      DD = TMath::Sqrt(DD);
      dist = -DD;
      nsoli = 2;
      t1 = -BB + DD;
      t2 = -BB - DD;
      c1 = *this + t1*nd;
      c2 = *this + t2*nd;
    }
    else {
      dist = TMath::Sqrt(-DD);
      if (-DD<epsilon) {
        //here there is almost an intersection. We accept it with caution.
        nsoli = 1;
        t1 = -BB;
        t2 = t1;
        c1 = *this + t1*nd;
        c2 = c1;
      }
      else nsoli = 0;
    }
  }
  //Are the solutions inside the truncated cone ?
  nsolb = 0;
  if (nsoli) {
    x3    = s - c1;
    h1    = H - x3*a;
    dist1 = TMath::Max(-h1,h1-h);
    if (dist1<epsilon) {
      nsolb +=1;
      if (dist1<=zero) q1 = good;
      else {
        //we push the point towards the truncated cone
        q1 = limit;
        x3.Unit();
        CC = TMath::Sqrt(un + ta*ta);
        if (h1<zero) AA = -h1*CC     + dixm300;
        if (h1>h)    AA = -(h1-h)*CC - dixm300;
        c1 = c1 + AA*x3;
      }
    }
  }
  if (nsoli==2) {
    x3    = s - c2;
    h2    = H - x3*a;
    dist2 = TMath::Max(-h2,h2-h);
    if (dist2<epsilon) {
      nsolb +=1;
      if (dist2<=zero) q2 = good;
      else {
        //we push the point towards the truncated cone
        q2 = limit;
        x3.Unit();
        CC = TMath::Sqrt(un + ta*ta);
        if (h2<zero) AA = -h2*CC     + dixm300;
        if (h2>h)    AA = -(h2-h)*CC - dixm300;
        c2 = c2 + AA*x3;
      }
    }
  }
  //decides of the order in which solutions are presented
  if ((nsolb==1) && (q1==bad)) exchange = kTRUE;
  if (nsolb==2) {
    switch (q1) {
      case bad:
        break;
      case limit:
        switch (q2) {
      case bad:
        break;
      case limit:
      case good:
        if (t2>t1) exchange = kTRUE;
        break;
        }
        break;
      case good:
        switch (q2) {
      case bad:
        break;
      case limit:
      case good:
        if (t2>t1) exchange = kTRUE;
        break;
        }
        break;
    }
  }
  if (exchange) {
    AA    = t1;
    q0    = q1;
    BB    = dist1;
    x3    = c1;
    t1    = t2;
    q1    = q2;
    dist1 = dist2;
    c1    = c2;
    t2    = AA;
    q2    = q0;
    dist2 = BB;
    c2    = x3;
  }
  //do not accept solutions with negative or zero t values
  if ((q2!=bad) && (t2<=tcut)) nsolb -= 1;
  if ((q1!=bad) && (t1<=tcut)) nsolb -= 1;
  return dist;
}
Short_t T3Vector::DegMatrix(T33Matrix &M,T3Vector &y,Bool_t test) {
  //Solve the equation A*x = 0 assuming that matrix A is degenerate. There are
  //3 ways to get vector x. We use the 3, with a ponderate method. The weights
  //used are the sinus squared of the angle between 2 vectors. It is natural,
  //since when this sinus is 0, the solution is undefined.
  //
  //  In the following, x is "this", y is the second argument.
  //
  //Put test to kTRUE if you want result to be checked.
  //
  //If 0 returned, it means that matrix M is not degenerate, so that the
  //               problem A*x = 0 has no solution other than x = 0.
  //               x = y = 0 is returned
  //If 1 returned, matrix M is once degenerate, normal case. There is
  //               only 1 vector as solution x. Vector y is 0.
  //If 2 returned, matrix M twice degenerate. All vectors in a certain plane
  //               are solutions. We arbitrary return 2 orthogonal vectors in
  //               this plane as solution : vector x and y. Remember in that
  //               case that any linear combination of these 2 vectors is also
  //               a solution.
  //If 3 returned, matrix M is the 0 matrix. Any vector is solution, so that
  //               it does not make sense to return something in x or y. We
  //               return x=0 and y=0.
  //
  //When not 0, the return vectors are always normalized to 1
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t small  = 1.0e-5;
  const Double_t vsmall = 1.0e-24;
  const Double_t ltest  = 5.0*small;
  T3Vector x;
  Short_t deg = 1;
  Double_t a,b,c,d;
  Bool_t   b0,b1,b2;
  Bool_t   gen = kFALSE;
  Double_t w1,w2,w;
  Double_t w0 = zero;
  T3Vector x0,x1,x2,p;
  d = TMath::Abs(M.Det());
  x.Set(zero,zero,zero);
  y.Set(zero,zero,zero);
  if (d>small) {
    deg = 0;
    return deg;
  }
  M.GetLine(0,a,b,c);
  T3Vector m0(a,b,c);
  M.GetLine(1,a,b,c);
  T3Vector m1(a,b,c);
  M.GetLine(2,a,b,c);
  T3Vector m2(a,b,c);
  a = m0.Norm2Sqr();
  b = m1.Norm2Sqr();
  c = m2.Norm2Sqr();
  T3Vector vz(a,b,c);
  vz.Unit();
  a = vz(0);
  b = vz(1);
  c = vz(2);
  if (a<vsmall) {
    b0 = kFALSE;
    w0 = zero;
  }
  else {
    b0 = kTRUE;
    m0.Unit();
  }
  if (b<vsmall) {
    b1 = kFALSE;
    w1 = zero;
  }
  else {
    b1 = kTRUE;
    m1.Unit();
  }
  if (c<vsmall) {
    b2 = kFALSE;
    w2 = zero;
  }
  else {
    b2 = kTRUE;
    m2.Unit();
  }
  if (b0) {
    //m0 usable
    if (b1) {
      //m0 && m1 usable
      if (b2) {
        //m0 && m1 && m2 usable          : +0 +1 +2
        x0 = m0^m1;
        x1 = m0^m2;
        x2 = m1^m2;
        w0 = a*b*x0.Norm2Sqr();
        w1 = a*c*x1.Norm2Sqr();
        w2 = b*c*x2.Norm2Sqr();
        w  = w0 + w1 + w2;
        w0 /= w;
        w1 /= w;
        w2 /= w;
        x0.Unit();
        x1.Unit();
        x2.Unit();
        x0.RatherPositiv();
        x1.RatherPositiv();
        x2.RatherPositiv();
        x  = w0*x0 + w1*x1 + w2*x2;
      }
      else {
        //m0 && m1 usable, but not m2    : +0 +1 -2
        x0 = m0^m1;
        w0 = x0.Norm2Sqr();
        if (w0<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x0.Unit();
          x0.RatherPositiv();
          x = x0;
        }
      }
    }
    else {
      //m0 usable, but not m1
      if (b2) {
        //m0 && m2 usable, but not m1    : +0 -1 +2
        x1 = m0^m2;
        w1 = x1.Norm2Sqr();
        if (w1<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x1.Unit();
          x1.RatherPositiv();
          x = x1;
        }
      }
      else {
        //m0 usable but not m1, not m2   : +0 -1 -2
        deg = 2;
        p   = m0;
        gen = kTRUE;
      }
    }
  }
  else {
    //m0 unusable
    if (b1) {
      //m1 usable but not m0
      if (b2) {
        //m1 && m2 usable, not m0        : -0 +1 +2
        x2 = m1^m2;
        w2 = x2.Norm2Sqr();
        if (w2<vsmall) {
          deg = 2;
          p   = m1;
          gen = kTRUE;
        }
        else {
          x2.Unit();
          x2.RatherPositiv();
          x = x2;
        }
      }
      else {
        //m1 usable, not m0, m2          : -0 +1 -2
        deg = 2;
        p = m1;
        gen = kTRUE;
      }
    }
    else {
      //m0, m1 unusable
      if (b2) {
        //m2 usable, not m0,m1           : -0 -1 +2
        deg = 2;
        p = m2;
        gen = kTRUE;
      }
      else {
        //m0, m1, m2 unusable            : -0 -1 -2
        deg = 3;
      }
    }
  }
  if (gen) {
    x0.Set(un,zero,zero);
    x1.Set(zero,un,zero);
    x2.Set(zero,zero,un);
    a  = TMath::Abs(p*x0);
    b  = TMath::Abs(p*x1);
    c  = TMath::Abs(p*x2);
    if (a<=b) {
      if (a<=c) {
        x = p^x0;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
    else {
      if (b<=c) {
        x = p^x1;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
  }
  if (test) {
    switch (deg) {
    case 1:
      x0 = M*x;
      w0 = x0.Module();
      break;
    case 2:
      x0 = M*x;
      x1 = M*y;
      w0 = x0.Module() + x1.Module();
      break;
    default:
      break;
    }
    if (w0>ltest) {
      gMes->SetName("Unit");
      gMes->SetTitle("T3Vector");
      gMes->M(Error_M,1,"bad solution",w0);
    }
  }
  *this = x;
  return deg;
}
void T3Vector::Get(Double_t &a0,Double_t &a1,Double_t &a2) const {
  // Get components of vector into a0/a1/a2
  a0 = fV0;
  a1 = fV1;
  a2 = fV2;
}
void T3Vector::MinOrthoN(const T3Vector &v,Double_t &s,Double_t &c) {
  //  Change the vector *this in the MINIMAL WAY, in order that it gets
  //orthogonal to the vector v and that it has the length 1. On return,
  //s and c are the sin and cos of the angle between vectors *this (before it
  //moves) and v. v is any vector, not necessary unitary.
  //  It is to be noticed that this problem has not a unique solution when *this
  //and v are parallel ! It is up to you to have verified that *this and v are
  //not parallel. No warning message will be issued in case of the 2 vectors
  //parallel. On return, you will still get a *this vector which is orthogonal
  //to v and unitary, BUT its direction in the plane orthogonal to v will
  //be unpredictable (but always the same for the 2 same vectors). It will
  //be in the plane formed by v and the axis corresponding to the smallest
  //component of v. For instance if v.fV2 is the smallext component of the
  //vector v, *this will be in the plane (v,Oz).
  //
  const Double_t zero      = 0.0;
  const Double_t un        = 1.0;
  const Double_t deux      = 2.0;
  const Double_t small     = 1.0e-5;
  const Double_t VerySmall = 1.0e-12;
  Short_t k;
  Double_t f,y,s2,c2;
  T3Vector w;
  T3Vector a(v);
  T3Vector b(*this);
  a.Unit();
  b.Unit();
  w = a^b;
  s = w.Module();
  if (s>VerySmall) {
    f = un/s;
    c = a*b;
    if (s>small) {
      *this = f*(b - c*a);
    }
    else {
      //if s is small, use a better way of handling cosine
      y  = (s*s)/deux;
      y -= y*y;
      T3Vector z;
      z  = b - a;
      *this = f*(z + y*a);
      Unit();
      //we make it a second time !
      b  = *this;
      w  = a^b;
      s2 = w.Module();
      f  = un/s2;
      c2 = a*b;
      *this = f*(b - c2*a);
    }
  }
  else {
    k = a.Smallest();
    switch (k) {
    case 0:
      b.Set(un,zero,zero);
      break;
    case 1:
      b.Set(zero,un,zero);
      break;
    case 2:
      b.Set(zero,zero,un);
      break;
    }
    w = a^b;
    c = un;
    *this = w;
  }
}
Double_t T3Vector::Module() const {
  //Calculates the module of the 3-vector
  const Double_t VerySmall = 1.0e-306;
  Double_t sum,a0,a1,a2,f;
  f  = TMath::Abs(fV0) + TMath::Abs(fV1) + TMath::Abs(fV2);
  if (f>VerySmall) {
    a0 = fV0/f;   a1 = fV1/f;   a2 = fV2/f;
    sum = TMath::Sqrt(a0*a0 + a1*a1 + a2*a2);
    sum = sum*f;
  }
  else {
    sum = TMath::Sqrt(fV0*fV0 + fV1*fV1 +fV2*fV2);
  }
  return sum;
}
Double_t T3Vector::Norm2Sqr() const {
  //Calculates the scalar product of the vector with itself
  Double_t sum;
  sum = fV0*fV0 + fV1*fV1 +fV2*fV2;
  return sum;
}
void T3Vector::Print() const {
  //Prints the 3-vector
  cout << *this << endl;
}
void T3Vector::RatherPositiv() {
  //If the direction of a vector is free, chooses it rather positiv !
  const Double_t zero = 0.0;
  Double_t d;
  d = fV0 + fV1 + fV2;
  if (d<zero) {
    fV0 = - fV0;
    fV1 = - fV1;
    fV2 = - fV2;
  }
}
Short_t T3Vector::Smallest() const {
  //Finds which of the 3 components fV0,fV1,fV2 is the smallest in absolute value
  Short_t k;
  Double_t a0,a1,a2;
  a0 = TMath::Abs(fV0);
  a1 = TMath::Abs(fV1);
  a2 = TMath::Abs(fV2);
  if (a0<a1) {
    if (a0<a2) {
      k = 0;
    }
    else {
      k = 2;
    }
  }
  else {
    if (a1<a2) {
      k = 1;
    }
    else {
      k = 2;
    }
  }
  return k;
}
Double_t T3Vector::TriangleSurface(const T3Vector &v2,const T3Vector &v3) const {
  //Calculates the surface of the triangle formed by this, v2 and v3
  Double_t s;
  T3Vector w1(0);
  T3Vector w2(0);
  T3Vector w(0);
  w1 = v2 - *this;
  w2 = v3 - *this;
  w  = w1^w2;
  s  = 0.5*w.Module();
  return s;
}
void T3Vector::Unit() {
  //Makes the length of the vector unity.
  const Double_t VerySmall = 1.0e-306;
  Double_t l;
  l = Module();
  if (l>VerySmall) {
    fV0 = fV0/l;
    fV1 = fV1/l;
    fV2 = fV2/l;
  }
  else {
    const Double_t zero = 0.0;
    const Double_t un   = 1.0;
    Double_t d0,d1,d2;
    d0 = TMath::Abs(fV0);
    d1 = TMath::Abs(fV1);
    d2 = TMath::Abs(fV2);
    if (d0>=d1)
      if (d0>=d2) {
        fV0 = TMath::Sign(un,fV0);
        fV1 = zero;
        fV2 = zero;
      }
      else {
        fV0 = zero;
        fV1 = zero;
        fV2 = TMath::Sign(un,fV2);
      }
    else
      if (d1>=d2) {
        fV0 = zero;
        fV1 = TMath::Sign(un,fV1);
        fV2 = zero;
      }
      else {
        fV0 = zero;
        fV1 = zero;
        fV2 = TMath::Sign(un,fV2);
      }
  }
}
Double_t T3Vector::UnitVCos(const T3Vector &v) const {
  //  Returns the cos of the angle between *this and v.
  //  *this and v are assumed to be unit vector !
  return v*(*this);
}
Double_t T3Vector::UnitVSin(const T3Vector &v) const {
  //  Returns the sin of the angle between *this and v. This sin is always positiv
  //because the angle between 2 vectors is always between 0 and 180 degrees.
  //  *this and v are assumed to be unit vector !
  T3Vector c;
  c = v^(*this);
  return c.Module();
}
Double_t T3Vector::VCos(const T3Vector &v) const {
  //  Returns the cos of the angle between *this and v.
  //  *this and v are assumed to be unit vector !
  T3Vector a(v);
  T3Vector b(*this);
  a.Unit();
  b.Unit();
  return a*b;
}
Double_t T3Vector::VSin(const T3Vector &v) const {
  //  Returns the sin of the angle between *this and v. This sin is always positiv
  //because the angle between 2 vectors is always between 0 and 180 degrees.
  //  *this and v are any whatsoever. They are not assumed to be unit vector !
  T3Vector c;
  T3Vector a(v);
  T3Vector b(*this);
  a.Unit();
  b.Unit();
  c = a^b;
  return c.Module();
}
T3Vector T3Vector::operator+(const T3Vector &v) const {
  //+ with other 3_vector
  T3Vector v4;
  v4.fV0 = fV0 + v.fV0;
  v4.fV1 = fV1 + v.fV1;
  v4.fV2 = fV2 + v.fV2;
  return v4;
}
T3Vector T3Vector::operator-(const T3Vector &v) const {
  //- with other 3_vector
  T3Vector v4;
  v4.fV0 = fV0 - v.fV0;
  v4.fV1 = fV1 - v.fV1;
  v4.fV2 = fV2 - v.fV2;
  return v4;
}
T3Vector& T3Vector::operator+=(const T3Vector &v) {
  //Add itself with a 3-vector
  fV0 += v.fV0;
  fV1 += v.fV1;
  fV2 += v.fV2;
  return *this;
}
T3Vector& T3Vector::operator-=(const T3Vector &v) {
  //Add itself with a 3-vector
  fV0 -= v.fV0;
  fV1 -= v.fV1;
  fV2 -= v.fV2;
  return *this;
}
Double_t operator*(const T3Vector &v1, const T3Vector &v2) {
  //Scalar product.
  Double_t sum;
  sum = v1.fV0*v2.fV0 + v1.fV1*v2.fV1 + v1.fV2*v2.fV2;
  return sum;
}
T3Vector operator*(const Double_t x, const T3Vector &v) {
  //Multiplication by a real number
  return T3Vector(x*v.fV0,x*v.fV1,x*v.fV2);
}
T3Vector operator*(const T3Vector&v, const Double_t x) {
  //Multiplication by a real number
  return T3Vector(x*v.fV0,x*v.fV1,x*v.fV2);
}
T3Vector operator*(const T33Matrix &m, const T3Vector &v) {
  //Multiplication of a matrix by a column vector
  T3Vector w;
  w.fV0 = m.fM00*v.fV0  +  m.fM01*v.fV1  +  m.fM02*v.fV2;
  w.fV1 = m.fM10*v.fV0  +  m.fM11*v.fV1  +  m.fM12*v.fV2;
  w.fV2 = m.fM20*v.fV0  +  m.fM21*v.fV1  +  m.fM22*v.fV2;
  return w;
}
T3Vector operator*(const T3Vector &v, const T33Matrix &m) {
  //Multiplication of a line vector (transposed vector) by a matrix
  T3Vector w;
  w.fV0 = m.fM00*v.fV0  +  m.fM10*v.fV1  +  m.fM20*v.fV2;
  w.fV1 = m.fM01*v.fV0  +  m.fM11*v.fV1  +  m.fM21*v.fV2;
  w.fV2 = m.fM02*v.fV0  +  m.fM12*v.fV1  +  m.fM22*v.fV2;
  return w;
}
T3Vector T3Vector::operator^(const T3Vector &v) const {
  //Compute the vector product
  T3Vector v4;
  Double_t a1,a2,a3,b1,b2,b3;
  a1 = fV0;
  a2 = fV1;
  a3 = fV2;
  b1 = v.fV0;
  b2 = v.fV1;
  b3 = v.fV2;
  v4.fV0 = a2*b3 - a3*b2;
  v4.fV1 = a3*b1 - a1*b3;
  v4.fV2 = a1*b2 - a2*b1;
  return v4;
}
T3Vector& T3Vector::operator=(const T3Vector &v) {
  //defines = for 3-vectors
  if (this != &v) {
    fV0 = v.fV0;
    fV1 = v.fV1;
    fV2 = v.fV2;
  }
  return *this;
}
Double_t &T3Vector::operator()(Int_t ind) const {
  //access to components
  Double_t *p;
  switch (ind) {
    case 0:
      p = (Double_t *)&fV0;
      break;
    case 1:
      p = (Double_t *)&fV1;
      break;
    case 2:
      p = (Double_t *)&fV2;
      break;
    default:
      gMes->SetName("Operator()");
      gMes->SetTitle("T3Vector");
      gMes->M(Error_M,1,"A T3Vector has 3 components : [0,1,2]");
      p = (Double_t *)&fV2;
      break;
  }
  return *p;
}
Double_t &T3Vector::operator()(Int_t index) {
  //access to components
  return (Double_t&)((*(const T3Vector *)this)(index));
}
istream &operator>>(istream &is,T3Vector &v) {
  //Allows to input a 3-vector
  char c;
  cin >> c >> v.fV0;
  cin >> c >> v.fV1;
  cin >> c >> v.fV2;
  cin >> c;
  return ( is );
}
ostream &operator<<(ostream &os,const T3Vector &v) {
  //Allows to output a 3-vector
  os << "( ";
  os.width(15);
  os.precision(8);
  os << v.fV0 << ", ";
  os.width(15);
  os.precision(8);
  os << v.fV1 << ", ";
  os.width(15);
  os.precision(8);
  os << v.fV2 << " )";
  return ( os );
}
