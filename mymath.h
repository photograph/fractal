#ifndef _MY_MATH_H_
#define _MY_MATH_H_

class Math {
  MyComplex z;
  MyComplex c;
  MyComplex p;
public:
  Math() { ; }
  Math(MyComplex zIn, MyComplex cIn=MyComplex(0,0), MyComplex pIn=MyComplex(0,0)) {
    z = zIn;
    c = cIn;
    p = pIn;
  }
  MyComplex Calc(int type) {
    if (type == 2) {
      return Func2();
    } else if (type == 3) {
      return Func3();
    } else {
      return z;
    }
  }
  MyComplex CalcDash(int type) {
    if (type == 2) {
      return Func2dash();
    } else if (type == 3) {
      return Func3dash();
    } else {
      return z;
    }
  }
  MyComplex Func2() {
    MyComplex ret = (z-1)*(z-c+0.5)*(z+c+0.5);
    return ret;
  }
  MyComplex Func2dash() {
    MyComplex ret = (z*z*3-(c*c+0.75+p));
    return ret;
  }
  MyComplex Func3() {
    MyComplex ret = z.Exp()+z.Exp(2).Inv()-z.Cos()*2-6;
//z.Diagnosis();printf(":");
//ret.Diagnosis();
    return ret;
  }
  MyComplex Func3dash() {
    MyComplex ret = z.Exp()+z.Exp(2).Inv()*0.69314718+z.Sin()*2+p;
    return ret;
  }
};
#endif
