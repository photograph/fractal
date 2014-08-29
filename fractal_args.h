#ifndef _FRACTAL_ARGS_H_
#define _FRACTAL_ARGS_H_

#define MAX_PARAM 20

#define MAX_TYPE_NAME 256
typedef struct tagFractalTypes{
  int type;
  char typeName[MAX_TYPE_NAME ];
} FractalTypes;
FractalTypes fractalTypes[] = {
{1, "mandelbrot"},
{2, "Mandelbrot Z"},
{3, "Mandelbrot burning ship"},
{4, "Mandelbrot burning ship Julia"},
{5, "Mandelbrot Z^4"},
{6, "Mandelbrot Z^3"},
{7, "Mandelbrot Julia"},
{8, "Mandelbrot Z Julia"},
{9, "Newton"},
{10, "Newton Secant"},
{11, "Newton (z-1)(z-c+0.5)(z+c+0.5)"},
{12, "Newton e^z+2^(-z)-2cos(z)-6"}
};

class FractalArgs {
private:
  double cxMin, cxMax, cyMin, cyMax;
  int maxL;
  double th;
  int type;
  int arg_error;
  int verbose;
  int debug;
  int imgW, imgH;
  double px, py;
  int colorType; // 1: discrete; 2: continous
  int escapeMethod; // 1: x*x+y*y; 2: |x|
  int numParam;
  double params[MAX_PARAM];

public:
  FractalArgs() {
   Init();
  }
  FractalArgs(int argc, char **argv) {
    // set defautl values first
    Init();
    if (argc <= 1) {
      Usages();
      type = -1;
      arg_error = 10000;
    } else {
      char **p = &argv[1];
if (debug)
printf("argc=%d\n", argc);
      for (int i=0; i<argc-1; i++) {
if (debug)
printf("i=%d: analyzing %s\n", i, p[i]);
        if (strcmp(p[i], "-g") == 0) {
           if (i<argc-4) {
             cxMin = atof(p[i+1]);
             cxMax = atof(p[i+2]);
             cyMin = atof(p[i+3]);
             cyMax = atof(p[i+4]);
             i += 4;
           } else {
             printf("wrong -g option: need 4 parameters: cxMin cxMax cyMin cyMax\n");
             arg_error = 1;
             break;
           }
        } else if (strcmp(p[i], "-imgDim") == 0) {
           if (i<argc-2) {
             imgW = atof(p[i+1]);
             imgH = atof(p[i+2]);
             i += 2;
           } else {
             printf("wrong -imgDim option: need 2 parameters: cxMin cxMax cyMin cyMax\n");
             arg_error = 2;
             break;
           }
        } else if (strcmp(p[i], "-p") == 0) {
           if (i<argc-2) {
             px = atof(p[i+1]);
             py = atof(p[i+2]);
             i += 2;
           } else {
             printf("wrong -p option: need 2 parameters: cxMin cxMax cyMin cyMax\n");
             arg_error = 3;
             break;
           }
        } else if (strcmp(p[i], "-params") == 0) {
           if (i<argc-2) {
             numParam = atoi(p[i+1]);
             if (numParam > MAX_PARAM) {
               printf("too many parameters! seeing:%d; maximum:%d\n",numParam, MAX_PARAM);
               arg_error = 9;
               break;
             }
             if (i>(argc-(numParam+1))) {
               printf("wrong -params option: should be num param1, param2,...\n");
               printf("numParam=%d,argc=%d,i=%d\n", numParam, argc,i);
               arg_error = 9;
               break;
             }
             for (int ip=0; ip<numParam; ip++) {
               params[ip] = atof(p[i+1+ip+1]);
             }
             i += (numParam+1);
           } else {
             printf("wrong -params option: need at least 2 parameters: num param1, param2,...\n");
             arg_error = 9;
             break;
           }
        } else if (strcmp(p[i], "-escapeMethod") == 0) {
           if (i<argc-1) {
             escapeMethod = atoi(p[i+1]);
             if (escapeMethod != 1 && escapeMethod != 2 && escapeMethod != 3) {
               printf("wrong -escapeMethod option: should be 1 2 or 3; seeing %d\n", escapeMethod);
               break;
             }
             i++;
           } else {
             printf("wrong -escapeMethod option: need 1 parameter: escapeMethod\n");
             arg_error = 4;
             break;
           }
        } else if (strcmp(p[i], "-color") == 0) {
           if (i<argc-1) {
             colorType = atoi(p[i+1]);
             i++;
           } else {
             printf("wrong -color option: need 1 parameter: color\n");
             arg_error = 4;
             break;
           }
        } else if (strcmp(p[i], "-th") == 0) {
           if (i<argc-1) {
             th = atof(p[i+1]);
             i++;
           } else {
             printf("wrong -th option: need 1 parameter: th\n");
             arg_error = 5;
             break;
           }
        } else if (strcmp(p[i], "-type") == 0) {
           if (i<argc-1) {
             type = atoi(p[i+1]);
             if (GetTypeNum()< 0) {
               printf("unknow -type option: seeing %d, but only know the following.\n", type);
               for (unsigned int i=0; i<sizeof(fractalTypes)/sizeof(struct tagFractalTypes); i++) {
                 printf("type %d: %s\n", fractalTypes[i].type, fractalTypes[i].typeName);
               }
               arg_error = 7;
               break;
             }
             i++;
           } else {
             printf("wrong -type option: need 1 parameter: type\n");
             arg_error = 7;
             break;
           }
        } else if (strcmp(p[i], "-maxL") == 0) {
           if (i<argc-1) {
             maxL = atoi(p[i+1]);
             i++;
           } else {
             printf("wrong -g option: need 1 parameter: maxL\n");
             arg_error = 8;
             break;
           }
        } else if (strcmp(p[i], "-verbose") == 0) {
           verbose = 1;
        } else if (strcmp(p[i], "-debug") == 0) {
           debug = 1;
        } else {
          printf("unknown option: %s\n", p[i]);
          arg_error = 100;
          break;
        } 
      }
    }
  }
  void GetImageDimension(int *imgWOut, int *imgHOut) {
    *imgWOut = imgW;
    *imgHOut = imgH;
  }
  void GetParams(int *num, double *p) {
    *num = numParam;
    for (int i=0; i<numParam; i++) {
      p[i] = params[i];
    }
  }
  double Px() { return px; }
  double Py() { return py; }
  int HasError() {
    return (arg_error ? 1 : 0);
  }
  int EscapeMethod() {
    return escapeMethod;
  }
  int GetTypeNum() {
    int ret = -1;
    for (unsigned int i=0; i<sizeof(fractalTypes)/sizeof(struct tagFractalTypes); i++) {
      if (type == fractalTypes[i].type) {
        ret = i;
      }
    }
    return ret;
  }
  void Usages() {
    printf("Options:\n");
    printf("  -g xMin xMax yMin yMax\n");
    printf("  -type [1-10]\n");
    printf("  -imgDim width height\n");
    printf("  -color [1-3]\n");
    printf("  -maxL num(maximum loop count)\n");
    printf("  -th val(the threshold for escape)\n");
    printf("  -escapeMethod [1-3](1: loop count; 2: minZ; 3: maxZ)\n");
    printf("  -params num param1 param2 ...\n");
    printf("  -debug\n");
    printf("  -verbose\n");
  }
  void Diagnosis() {
    int i = GetTypeNum();
    if (i >= 0) {
      printf("fractal type: %s\n", fractalTypes[i].typeName);
    }
    printf("imgW=%d,imgH=%d\nverbose=%d, debug=%d, escapeMethod=%d\ntype=%d; th=%f,maxL=%d,color=%d\nrange(xMin=%f,xMax=%f,yMin=%f,yMax=%f)\nP(%f,%f)\n",
         imgW, imgH,
         verbose, debug, escapeMethod,
         type, th, maxL, colorType, cxMin, cxMax, cyMin, cyMax, px, py);
    if (numParam != 0) {
      for (i=0; i<numParam; i++) {
        printf("param[%d]=%f\n", i, params[i]);
      }
    }
  }
  int Verbose() { return verbose; }
  int Debug() { return debug; }
  void Init() {
    type = 1;
    th = 4;
    maxL = 100;
    cxMin = -2.0;
    cxMax = 2.0;
    cyMin = -2.0;
    cyMax = 2.0;
    arg_error = 0;
    verbose = 0;
    debug = 0;
    imgW = imgH = 2048;
    px = py = 0.0;
    colorType = 1;
    escapeMethod = 1;
    numParam = 0;
  }
  int ColorType() {
    return colorType;
  }
  void GetRange(double *cxMinOut, double *cxMaxOut, double *cyMinOut, double *cyMaxOut) {
    *cxMinOut = cxMin;
    *cxMaxOut = cxMax;
    *cyMinOut = cyMin;
    *cyMaxOut = cyMax;
  }
  double Th() {
    return th;
  }
  int MaxL() {
    return maxL;
  }
  int Type() {
    return type;
  }
  int IsJulia() { return ((type == 4 || type == 6 || type == 7 || type == 8) ? 1 : 0); }
};

#endif
