#ifndef _MY_COLOR_H_
#define _MY_COLOR_H_

class MyColor {
private:
  double r, g, b;
public:
  MyColor() {
    Set(0, 0, 0);
  }
  MyColor(double rr, double gg, double bb) {
    Set(rr, gg, bb);
  }
  // copy constructor
  MyColor(const MyColor &obj) {
    r = obj.R();
    g = obj.G();
    b = obj.B();
  }
  MyColor& operator=(const MyColor &obj) {
    Set(obj.R(), obj.G(), obj.B());
    return (*this);
  }
  void Set(double rr, double gg, double bb) {
    r = rr; g = gg; b = bb;
  }
  double R() const { return r; }
  double G() const { return g; }
  double B() const { return b; }
};
class Colors {
private:
  MyColor *colors;
  int size;
  void initColorTbl(MyColor *colors, int size)
  {
    int div = 6;
    double numG = size/div;
    int interval = 256/numG;
    for (int i=0; i<size/div; i++) {
      int val = (i+1)*interval;
      if (val > 255) val = 255;
      colors[i*6+0] = MyColor(val,   0,   0); 
      colors[i*6+1] = MyColor(  0, val,   0); 
      colors[i*6+2] = MyColor(  0,   0, val); 
      colors[i*6+3] = MyColor(val, val,   0); 
      colors[i*6+4] = MyColor(val,   0, val); 
      colors[i*6+5] = MyColor(  0, val, val); 
    }
  }
public:
  Colors() {
    colors = NULL;
    size = 0;
  }
  Colors(int sizeIn) {
    colors = new MyColor[sizeIn];
    size = sizeIn;
    initColorTbl(colors, size);
  }
  ~Colors() {
    if (size > 0 && colors != NULL) {
      delete [] colors;
    }
  }
  MyColor GetColor(int index) {
    if (index >= 0 and index < size) {
      return colors[index];
    } else if (index > size) {
      return colors[index%size];
    }
    return colors[0];
  }
  MyColor Interpolate(double i) {
    MyColor ret(colors[0]);
    if (i > 0 && i<(size-1)) {
      int i1 = (int)i;
      int i2 = (int)(i+1);
      double delta = i-i1;
      ret.Set(colors[i1].R()+delta*(colors[i2].R()-colors[i1].R()),
              colors[i1].G()+delta*(colors[i2].G()-colors[i1].G()),
              colors[i1].B()+delta*(colors[i2].B()-colors[i1].B()));
    } else if (i >= (size-1)) {
      ret = colors[size-1];
    }
    return ret;
  }
  void Diagnosis() {
    for (int cc=0; cc<size; cc++) {
      MyColor c = GetColor(cc);
      printf("%f,%f,%f\n",c.R(),c.G(),c.B());
    }
  }
};
#endif
