#ifndef _MY_COMPLEX_H_
#define _MY_COMPLEX_H_

class MyComplex {
private:
  double x, y;
public:
  MyComplex() {
    Set(0.0, 0.0);
  }
  MyComplex(double xI, double yI) {
    Set(xI, yI);
  }
  // copy constructor
  MyComplex(const MyComplex &obj) {
    x = obj.X();
    y = obj.Y();
  }
  MyComplex& operator=(const MyComplex &obj) {
    Set(obj.X(), obj.Y());
    return (*this);
  }
  void Set(double xI, double yI) {
    x = xI; y = yI;
  }
  double X() const { return x; }
  double Y() const { return y; }
  const MyComplex operator+(const MyComplex &obj) const {
    MyComplex ret(x, y);
    ret.Set(x+obj.X(), y+obj.Y());
    return (ret);
  }
  const MyComplex operator+(const double a) const {
    MyComplex ret;
    ret.Set(x+a, y);
    return (ret);
  }
  const MyComplex operator-(const MyComplex &obj) const {
    MyComplex ret;
    ret.Set(x-obj.X(), y-obj.Y());
    return (ret);
  }
  const MyComplex operator-(const double a) const {
    MyComplex ret;
    ret.Set(x-a, y);
    return (ret);
  }
  const MyComplex operator*(const MyComplex &obj) const {
    MyComplex ret;
    ret.Set((x*obj.X()- y*obj.Y()), (x*obj.Y()+y*obj.X()));
    return (ret);
  }
  const MyComplex operator*(const double a) const {
    MyComplex ret;
    ret.Set(x*a, y);
    return (ret);
  }
  const MyComplex operator/(const MyComplex &obj) const {
    MyComplex ret;
    double temp = obj.X()*obj.X() + obj.Y()*obj.Y();
    if (temp > 1e-12) {
      ret.Set(((x*obj.X()+y*obj.Y())/temp), ((y*obj.X()-x*obj.Y())/temp));
    }
    return (ret);
  }
  const MyComplex operator/(const double a) const {
    MyComplex ret;
    if (a > 1e-12) {
      ret.Set(x/a, y/a);
    }
    return (ret);
  }
  double abs2() {
    return (x*x+y*y);
  }
  void Diagnosis() {
    printf("(%f,%f)",x,y);
  }
  MyComplex Exp() {
    MyComplex ret(exp(x)*cos(y), exp(x)*sin(y));
    return ret;
  }
  MyComplex Exp(double v) {
    // v^z = exp(z*ln(v))
    MyComplex z(x, y);
    z = z*log(v);
    return z.Exp();
  }
  MyComplex Inv() {
    double a2 = abs2();
    MyComplex ret(x/a2, -y/a2);
    return ret;
  }
  MyComplex Sin() {
    MyComplex z(-y, x);     // i*theta
    MyComplex z1 = z.Exp();
    z.Set(y, -x);           // -i*theta)
    z1 = (z1+z)/2;
    return z1;
  }
  MyComplex Cos() {
    MyComplex z(-y, x);     // i*theta
    MyComplex z1 = z.Exp();
    z.Set(y, -x);           // -i*theta)
    z1 = (z1-z.Exp())/2;
    z.Set(0,1);
    z1 = z1/z;              // z1/i
    return z1;
  }
  MyComplex Ln() {
    double r = sqrt(x*x+y*y);
    double theta = atan2(x, y);
    MyComplex ret(log(r), theta);
    return ret;
  }
};
#endif

