#include <Magick++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;
using namespace Magick;

#include "mycolor.h"
#include "mycomplex.h"
#include "fractal_args.h"
#include "mymath.h"

#define STDOUT_EN

#define myabs(a) (((a)>0) ? a : -a)

class Fractal {
private:
  double cx, cy;
  int type;
  double th;
  int escapeMethod;
  double minZ, maxZ;
  double params[MAX_PARAM];
  int numParam;

  int escape(double x, double y);
public:
  Fractal(double cxIn, double cyIn, int typeIn, double thIn, int escapeMethodIn=1) {
    cx = cxIn;
    cy = cyIn;
    type = typeIn;
    th = thIn;
    escapeMethod = escapeMethodIn;
    minZ = th;
    maxZ = 0;

    if (type >= 9 && type <= 10) {
      th *= th;
    }
  }
  void SetParams(int num, double *p) {
    if (num > MAX_PARAM) {
      printf("too many parameters! seeing:%d; maximum:%d\n",num, MAX_PARAM);
    }
    for (int i=0; i<num; i++) {
      params[i] = p[i];
    }
  }
  int calc(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcMandelbrotZ(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcMandelbrot(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcMandelbrot4(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcMandelbrot3(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcMandelbrotBurningShip(double x0, double y0, double x02, double y02, double *x, double *y);
  void calcNewton(double x0, double y0, double x02, double y02, double *x, double *y, int subType=0);
  void calcNewtonSecant(double x0, double y0, double x02, double y02, double *x, double *y);
  double MinZ() { return minZ; }
  double MaxZ() { return maxZ; }
};
int Fractal::escape(double x, double y)
{
  if (escapeMethod == 2) {
    return (myabs(x)>th?1:0);
  } else if (escapeMethod == 3) {
    return (myabs(y)>th?1:0);
  } else {
    return (myabs(y)>th?1:0);
  }
}
int Fractal::calc(double x0, double y0, double x02, double y02, double *x, double *y)
{
  int ret = escape(x02,y02);
  if (type >= 9 && type <= 12) {
    MyComplex z(x0, y0);
    if (type >= 11 && type <= 12) {
      MyComplex p(cx, cy);
      MyComplex t(params[0], params[1]); // this is c
      Math math(z, p, t);
      ret = (sqrt(math.Calc(type-9).abs2())<th ? 1 : 0);
    } else {
      MyComplex z2 = z*z;
      MyComplex fz = (z2*z-z*2-5);
      ret = ((fz.abs2())<th ? 1 : 0);
    }
//fz.Diagnosis();
//printf("r=%d;%f,%f",ret,fz.abs2(),th);
  }
  if (!ret) {
    if (minZ > (x02+y02)) {
      minZ = (x02+y02);
    }
    if (maxZ < (x02+y02)) {
      maxZ = (x02+y02);
    }
    if (type == 1 || type == 7) {
      calcMandelbrot(x0, y0, x02, y02, x, y);
    } else if (type == 2 || type == 8) {
      calcMandelbrotZ(x0, y0, x02, y02, x, y);
    } else if (type == 3 || type == 4) {
      calcMandelbrotBurningShip(x0, y0, x02, y02, x, y);
    } else if (type == 5) {
      calcMandelbrot4(x0, y0, x02, y02, x, y);
    } else if (type == 6) {
      calcMandelbrot3(x0, y0, x02, y02, x, y);
    } else if (type >= 9 && type <= 12) {
      calcNewton(x0, y0, x02, y02, x, y, ((type==11)?2:((type==12)?3: 1)));
    } else if (type == 10) {
      calcNewtonSecant(x0, y0, x02, y02, x, y);
    } else {
      printf("unknown type: %d.\n", type);
    }
  }
  return ret;
}
void Fractal::calcNewtonSecant(double x0, double y0, double x02, double y02, double *x, double *y)
{
  MyComplex z(x0, y0);
  MyComplex p(cx, cy);
  MyComplex z2 = z*z;
  MyComplex fz = (z2*z-z*2-5);
  MyComplex zplh = z+params[0];
  MyComplex fzplh = zplh*zplh*zplh-zplh*2-5;
  MyComplex fdashz = (fzplh-fz)/params[0];
  MyComplex znew = z-fz/(fdashz+p);
  *x = znew.X();
  *y = znew.Y();
}
void Fractal::calcNewton(double x0, double y0, double x02, double y02, double *x, double *y, int subType)
{
  MyComplex z(x0, y0);
  MyComplex p(cx, cy);
//z.Diagnosis();
//p.Diagnosis();
  if (subType == 1) {
    MyComplex z2 = z*z;
    MyComplex znew = z-(z2*z-z*2-5)/(z2*3-2+p);
    *x = znew.X();
    *y = znew.Y();
  } else if (subType == 2) {
    MyComplex t(params[0], params[1]); // this is c
    Math math(z, p, t);
//printf("t=");t.Diagnosis();
    MyComplex ret = z-math.Func2()/math.Func2dash();
//ret.Diagnosis();
    *x = ret.X();
    *y = ret.Y();
  } else if (subType >= 3) {
    MyComplex t(params[0], params[1]); // this is c
    Math math(z, p, t);
    MyComplex ret = math.Calc(subType);
    ret = z-ret/math.CalcDash(subType);
//ret.Diagnosis();
    *x = ret.X();
    *y = ret.Y();
  }
}
void Fractal::calcMandelbrot(double x0, double y0, double x02, double y02, double *x, double *y)
{
  // znew = z*z + c
  *x = x02-y02+cx;
  *y = 2.0*x0*y0+cy;
}
void Fractal::calcMandelbrot3(double x0, double y0, double x02, double y02, double *x, double *y)
{
    // znew = z*z + c
  double c = x02-y02;
  double d = 2.0*x0*y0;
  *x = c*x0-d*y0+cx;
  *y = d*x0+c*y0+cy;
}
void Fractal::calcMandelbrot4(double x0, double y0, double x02, double y02, double *x, double *y)
{
    // znew = z*z + c
  double c = x02-y02;
  double d = 2.0*x0*y0;
  double c2 = c*c;
  double d2 = d*d;
  *x = c2-d2+cx;
  *y = 2.0*c*d+cy;
}
void Fractal::calcMandelbrotBurningShip(double x0, double y0, double x02, double y02, double *x, double *y)
{
  // znew = z*z + c
  *x = x02-y02-cx;
  *y = myabs(2.0*x0*y0)-cy;
}
void Fractal::calcMandelbrotZ(double x0, double y0, double x02, double y02, double *x, double *y)
{
  // znnew = 1/(z*z)+c
  double temp = x02-y02;
  double temp1 = (temp*temp+4*x02*y02);
  if (temp1 > 1e-10) {
    *x = cx + temp/temp1;
    *y = cy - 2.0*x0*y0/temp1;
  }
}

#define MAX_COLOR 256
int main(int argc, char** argv)
{
  Colors colors(MAX_COLOR);

  FractalArgs fractalArgs(argc, argv);

  if (fractalArgs.Debug()) {
    fractalArgs.Diagnosis();
  }

  if (fractalArgs.HasError()) {
    return 1;
  }

  InitializeMagick(*argv);

  // dimension of image
  int imgW = 2048;
  int imgH = 2048;

  double cxMin, cxMax, cyMin, cyMax;

  double th = fractalArgs.Th();
  int maxL = fractalArgs.MaxL();
  int type = fractalArgs.Type();
  int escapeMethod = fractalArgs.EscapeMethod();

  int numParam = 0;
  double params[MAX_PARAM];

  if (type == 10) {
    fractalArgs.GetParams(&numParam, params);
    if (numParam != 1) {
      printf("wrong parameter for type 10. should be only 1, seeing %d\n", numParam);
    }
    if (params[0] == 0) {
      printf("wrong parameter for type 10. should be none zero, seeing %f\n", params[0]);
    }
  }

  if (type == 11 || type == 12) {
    fractalArgs.GetParams(&numParam, params);
    if (numParam != 2) {
      printf("wrong parameter for type 11/12. there should be 2, seeing %d\n", numParam);
    }
  }
  fractalArgs.GetRange(&cxMin, &cxMax, &cyMin, &cyMax);
  fractalArgs.GetImageDimension(&imgW, &imgH);

  // Create base image
  Image image(Geometry(imgW,imgH), "black");
  // Set the image type to TrueColor DirectClass representation.
  image.type(TrueColorType);

  // Ensure that there is only one reference to underlying image
  // If this is not done, then image pixels will not be modified.
  image.modifyImage();

  // Allocate pixel view
  Pixels view(image);

  PixelPacket *pixels = view.get(0,0,imgW,imgH);

  for (int i=0; i<imgH; i++) {
    double cy = cyMin+(cyMax-cyMin)*i/imgH;
    for (int j=0; j<imgW; j++) {
      double cx = cxMin+(cxMax-cxMin)*j/imgW;
      double x0 = 0.0;
      double y0 = 0.0;
      double x1 = 0.0, y1 = 0.0;
      if (type == 1) {
      } else if (type == 2 || type == 8 || type == 11 || type == 12) {
        x0 = cx;
        y0 = cy;
      } else if (fractalArgs.IsJulia()) {
        x0 = cx;
        y0 = cy;
        cx = fractalArgs.Px();
        cy = fractalArgs.Py();
      }
      Fractal fractal(cx, cy, type, th, escapeMethod);
      if (fractalArgs.IsJulia()) {
        cx = x0;
        cy = y0;
      }
      if (type == 10 || type == 11 || type == 12) {
        fractal.SetParams(numParam, params);
      }
      int k = 0;
      for (; k<maxL; k++) {
        double x2 = x0*x0;
        double y2 = y0*y0;
        if (fractal.calc(x0, y0, x2, y2, &x1, &y1)) {
//printf("k=%d ", k);
          break;
        }
//  if (fractalArgs.Debug()) {
//    printf("x1=%f,y1=%f\n", x1, y1);
//  }
        x0 = x1;
        y0 = y1;
      }
      double t = 0;
      if (k < maxL) {
        MyColor color=colors.GetColor((k%MAX_COLOR));
        if (fractalArgs.ColorType() == 2) {
          double dColor=k*MAX_COLOR/maxL;
          color=colors.Interpolate(dColor);
        } else if (fractalArgs.ColorType() == 3) {
          double dColor = fractal.MinZ();
          if (dColor > 5.0) dColor = 5.0;
          dColor = (dColor*k*MAX_COLOR/(5*5));
          t = dColor;
          color=colors.GetColor((((int)dColor)%MAX_COLOR));
        } else if (fractalArgs.ColorType() == 4) {
          double dColor = fractal.MaxZ();
          color=colors.GetColor((((int)dColor)%MAX_COLOR));
        }
        *pixels = Color((int)color.R(),(int)color.G(), (int)color.B());
        //*pixels = Color(((k*32)%256),((k*32)%256), ((k*32)%256));
      }
      ++pixels;
      if (k<maxL) {
        if (fractalArgs.Verbose())printf("%d",k);
        //if (fractalArgs.Verbose())printf("%5.1f",t);
      } else {
        if (fractalArgs.Verbose())printf("0");
      }
      if (fractalArgs.Verbose()) {
        if (j < imgW-1) {
          printf(",");
        }
      }
    }
    if (fractalArgs.Verbose()) {
      printf("\n");
    }
  }

  // Save changes to image.
  view.sync();

  image.write("x.png");
  return 0;
}
