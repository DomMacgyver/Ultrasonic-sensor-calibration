#ifndef Functions_h
#define Functions_h
#define swap(a, b) { double t = a; a = b; b = t; }
void lsrl(double x[], double y[],int dim, double values[])
{
  double b = 0.0;
  double d = 0.0;
  for(int i = 0;i < dim;i++)
  {
    b += x[i];
    d += x[i] * x[i];
  }
  double det = 1.0;
  double a = (double)dim;
  det /= (a * d - b * b);
  swap(a, d);

  a *= det;
  b *= -1 * det;
  d *= det;

  double y0 = 0.0;
  double y1 = 0.0;
  for(int i = 0;i < dim;i++)
  {
    y0 += y[i];
    y1 += y[i] * x[i];
  }

  double b0 = a * y0;
  b0 += b * y1;
  double b1 = b * y0;
  b1 += d * y1;

  double y_mean = y0;
  y_mean /= dim;
  double error = 0.0;
  double sst = 0.0;
  for(int i = 0;i < dim;i++)
  {
    double val = b1 * x[i];
    val += b0;
    val -= y[i];
    val *= val;
    error += val;
    
    val = y[i] - y_mean;
    val *= val;
    sst += val;
  }
  double r = 1 - (error / sst);

  values[0] = b0;
  values[1] = b1;
  values[2] = r;

}
#endif
