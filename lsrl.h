
float[] lsrl(float x[], float y[],int size)
{
  float b = 0.0;
  float d = 0.0;
  for(int i = 0;i < size;i++)
  {
    b += x[i];
    d += x[i] * x[i];
  }
  float det = 1.0;
  float a = size;
  det /= (a * d - b * b);
  swap(a, d);
  a *= det;
  b *= -1;
  b *= det;
  d *= det;
  
  float y0 = 0.0;
  float y1 = 0.0;
  for(int i = 0;i < size;i++)
  {
    y0 += y[i];
    y1 += y[i] * x[i];
  }

  float b0 = a * y0;
  b0 += b * y1;
  float b1 = b * y0;
  b1 += d * y1;
  float y_mean = y0;
  y_mean /= size;
  float error = 0.0;
  float sst = 0.0
  for(int i = 0;i < size;i++)
  {
    float val = b1 * x[i];
    val += b0;
    val -= y[i];
    val *= val;
    error += val;
    val = y[i];
    val -= y_mean;
    val *= val;
    sst += val;
  }
  float r = error;
  r /= sst;
  r *= -1;
  r += 1;
  
  float values[] = {b0, b1, r};
  return values;

}
