// Matlabから移植した関数の寄せ集め
#include <math.h>
#include "world.h"

// histc based on Matlab
// This function is hidden.
// length of i (Index) and y is the same.
void histc(double *x, int xLen, double *y, int yLen, int *index)
{
	int i;
	int count = 1;

	for(i = 0;i < yLen;i++)
	{
		index[i] = 1;
		if(y[i] >= x[0])
			break;
	}
	for(;i < yLen;i++)
	{
		if(y[i] < x[count]) index[i] = count;
		else
		{
			index[i] = count++;
			i--;
		}
		if(count == xLen) break;
	}
	count--;
	for(i++;i < yLen;i++) index[i] = count;
}

// interp1 by using linear interpolation
// This function is based on Matlab function that has the same name
void interp1(double *t, double *y, int iLen, double *t1, int oLen, double *y1)
{
	int i;
	double *h, *p, *s;
	int *k;
	h = (double *)malloc(sizeof(double) * (iLen-1));
	p = (double *)malloc(sizeof(double) * oLen);
	s = (double *)malloc(sizeof(double) * oLen);
	k = (int   *)malloc(sizeof(int)   * oLen);

	// 初期設定
	for(i = 0;i < iLen-1;i++) h[i] = t[i+1]-t[i];
	for(i = 0;i < oLen;i++) {p[i] = i; k[i] = 0;}

	histc(t, iLen, t1, oLen, k);

	for(i = 0;i < oLen;i++) s[i] = (t1[i] - t[k[i]-1]) / h[k[i]-1];

	for(i = 0;i < oLen;i++)
		y1[i] = y[k[i]-1] + s[i]*(y[k[i]] - y[k[i]-1]);

	free(k);
	free(s);
	free(p);
	free(h);
}



// decimate by using IIR and FIR filter coefficients
// x: Input signal whose length is xLen [sample]
// y: Output signal
long decimateForF0(double *x, int xLen, double *y, int r)
{
//	int r = 11;
	int nfact = 9; // 多分これは固定でOK
	double *tmp1, *tmp2;
	tmp1 = (double *)malloc(sizeof(double) * (xLen + nfact*2));
	tmp2 = (double *)malloc(sizeof(double) * (xLen + nfact*2));

	int i;
	for(i = 0;i < nfact;i++) tmp1[i] = 2*x[0] - x[nfact-i];
	for(i = nfact;i < nfact+xLen;i++) tmp1[i] = x[i-nfact];
	for(i = nfact+xLen;i < 2*nfact+xLen;i++) tmp1[i] = 2*x[xLen-1] - x[xLen-2 - (i-(nfact+xLen))];

	filterForDecimate(tmp1, 2*nfact+xLen, tmp2, r);
	for(i = 0;i < 2*nfact+xLen;i++) tmp1[i] = tmp2[2*nfact+xLen - i - 1];
	filterForDecimate(tmp1, 2*nfact+xLen, tmp2, r);
	for(i = 0;i < 2*nfact+xLen;i++) tmp1[i] = tmp2[2*nfact+xLen - i - 1];

	int nout = (int)(xLen/r) + 1;
	int nbeg = r - (r*nout - xLen);
	int count;

	for(i = nbeg, count = 0;i < xLen+nfact;i+=r, count++) y[count] = tmp1[i+nfact-1];

	free(tmp1); free(tmp2);
	return count;
}

// filter based on Matlab function
// x: Input signal whose length is xLen [sample]
// y: Output signal
void filterForDecimate(double *x, int xLen, double *y, int r)
{
	double w[3], wt;
	w[0] = w[1] = w[2] = 0.0;
	double a[3], b[2]; // フィルタ係数 (r依存)

	switch(r)
	{
	case 11: // fs : 44100
		a[0] = 2.450743295230728;
		a[1] = -2.06794904601978;
		a[2] = 0.59574774438332101;
		b[0] = 0.0026822508007163792;
		b[1] = 0.0080467524021491377;
		break;
	case 12: // fs : 48000
		a[0] = 2.4981398605924205;
		a[1] = -2.1368928194784025;
		a[2] = 0.62187513816221485;
		b[0] = 0.0021097275904709001;
		b[1] = 0.0063291827714127002;
		break;
	case 8: // fs : 32000
		a[0] = 2.2357462340187593;
		a[1] = -1.7780899984041358;
		a[2] = 0.49152555365968692;
		b[0] = 0.0063522763407111993;
		b[1] = 0.019056829022133598;
		break;
	case 6: // fs : 24000 and 22050
		a[0] = 1.9715352749512141;
		a[1] = -1.4686795689225347;
		a[2] = 0.3893908434965701;
		b[0] = 0.013469181309343825;
		b[1] = 0.040407543928031475;
		break;
	case 4: // fs : 16000
		a[0] = 1.4499664446880227;
		a[1] = -0.98943497080950582;
		a[2] = 0.24578252340690215;
		b[0] = 0.036710750339322612;
		b[1] = 0.11013225101796784;
		break;
	case 2: // fs : 8000
		a[0] = 0.041156734567757189;
		a[1] = -0.42599112459189636;
		a[2] = 0.041037215479961225;
		b[0] = 0.16797464681802227;
		b[1] = 0.50392394045406674;
	}

	for(int i = 0;i < xLen;i++)
	{
		wt = x[i] + a[0]*w[0]
				  + a[1]*w[1]
				  + a[2]*w[2];

		y[i] = b[0]*wt
			 + b[1]*w[0]
			 + b[1]*w[1]
			 + b[0]*w[2];

		w[2] = w[1];
		w[1] = w[0];
		w[0] = wt;
	}
}

// matlabに順ずる丸め
double round(double x)
{
	if(x > 0)
		return (int)(x+0.5);
	else
		return (int)(x-0.5);
}

// 差分
void diff(const double *x, int x_length, double *y) {
  for (int i = 0; i < x_length - 1; ++i) y[i] = x[i + 1] - x[i];
}


// サンプリング間隔が等間隔に限定し高速に動作するinterp1．
// 基本的には同じだが，配列の要素数を明示的に指定する必要がある．
void interp1Q(double x, double shift, const double *y, int x_length,
    const double *xi, int xi_length, double *yi) {
  double *xi_fraction = new double[xi_length];
  double *delta_y = new double[x_length];
  int *xi_base = new int[xi_length];

  double delta_x = shift;
  for (int i = 0; i < xi_length; ++i) {
    xi_base[i] = static_cast<int>((xi[i] - x) / delta_x);
    xi_fraction[i] = (xi[i] - x) / delta_x - xi_base[i];
  }
  diff(y, x_length, delta_y);
  delta_y[x_length - 1] = 0.0;

  for (int i = 0; i < xi_length; ++i)
    yi[i] = y[xi_base[i]] + delta_y[xi_base[i]] * xi_fraction[i];

  // Bug was fixed at 2013/07/14 by M. Morise
  delete[] xi_fraction;
  delete[] xi_base;
  delete[] delta_y;
}

// xorshift法と中心極限定理との組み合わせ
float randn(void)
{
	static unsigned int x = 123456789;
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;
	unsigned int t;
 	t = x ^ (x << 11);
	x = y; y = z; z = w;

	int i;
	unsigned int tmp;

	tmp = 0;
	for(i = 0;i < 12;i++)
	{
	 	t = x ^ (x << 11);
		x = y; y = z; z = w;
		w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		tmp += w>>4;
	}
	return (float)tmp / 268435456.0f - 6.0f;
}

// fftfilt関数の移植
// yは，fftl分の長さを確保すること．
/*
void fftfilt(double *x, int xlen, double *h, int hlen, int fftl, double *y)
{
	int i;
	fftw_plan			forwardFFT1, forwardFFT2, inverseFFT;
	fftw_complex *specx, *spech;
	double *input1, *input2;

	input1 = (double *)      malloc(sizeof(double) * fftl);
	input2 = (double *)      malloc(sizeof(double) * fftl);
	specx  = (fftw_complex *)malloc(sizeof(fftw_complex) * fftl);
	spech  = (fftw_complex *)malloc(sizeof(fftw_complex) * fftl);

	forwardFFT1 = fftw_plan_dft_r2c_1d(fftl, input1, specx, FFTW_ESTIMATE);
	forwardFFT2 = fftw_plan_dft_r2c_1d(fftl, input2, spech, FFTW_ESTIMATE);
	inverseFFT = fftw_plan_dft_c2r_1d(fftl, specx, y, FFTW_ESTIMATE);

	for(i = 0;i < xlen;i++) input1[i] = x[i]/(double)fftl;
	for(; i < fftl;i++) input1[i] = 0;
	for(i = 0;i < hlen;i++) input2[i] = h[i];
	for(; i < fftl;i++) input2[i] = 0;

	fftw_execute(forwardFFT1);
	fftw_execute(forwardFFT2);

	double tmpR, tmpI;
	for(i = 0;i <= fftl/2;i++)
	{
		tmpR = specx[i][0]*spech[i][0] - specx[i][1]*spech[i][1];
		tmpI = specx[i][0]*spech[i][1] + specx[i][1]*spech[i][0];
		specx[i][0] = tmpR;
		specx[i][1] = tmpI;
	}
	fftw_execute(inverseFFT);

	free(input1); free(input2);
	free(specx); free(spech);
	fftw_destroy_plan(forwardFFT1);
	fftw_destroy_plan(forwardFFT2);
	fftw_destroy_plan(inverseFFT);
}
*/
// 2次元配列 (n*n)の逆行列を計算．メモリは確保しておくこと
void inv(double **r, int n, double **invr)
{
	int i,j,k;
	double tmp;

	for(i = 0;i < n;i++)
	{
		for(j = 0;j < n;j++)
		{
			invr[i][j] = 0.0;
		}
	}
	for(i = 0;i < n;i++) invr[i][i] = 1.0;

	// 配列の初期化
	//
	for(i = 0;i < n;i++)
	{
		tmp = r[i][i]; r[i][i] = 1.0;
		for(j = 0;j <= i;j++) invr[i][j] /= tmp;
		for(;j < n;j++) r[i][j]	/= tmp;
		for(j = i+1;j < n;j++)
		{
			tmp = r[j][i];
			for(k = 0;k <= i;k++) invr[j][k] -= invr[i][k]*tmp;
			for(k--;k < n;k++) r[j][k] -= r[i][k]*tmp;
		}
	}

	// これで半分完了
	for(i = n-1;i >= 0;i--)
	{
		for(j = 0;j < i;j++)
		{
			tmp = r[j][i];
			for(k = 0;k < n;k++) invr[j][k] -= invr[i][k]*tmp;
		}
	}
}

double std2(double *x, int xLen)
{
	int i;
	double average, s;
	average = 0.0;
	for(i = 0;i < xLen;i++)
		average += x[i];
	average /= (double)xLen;

	s = 0.0;
	for(i = 0;i < xLen;i++)
		s += pow(x[i] - average, 2.0);
	s /= (double)(xLen-1);

	return sqrt(s);
}

int matlab_round(double x) {
  return x > 0 ? static_cast<int>(x + 0.5) : static_cast<int>(x - 0.5);
}

void fast_fftfilt(const double *x, int x_length, const double *h, int h_length,
    int fft_size, const ForwardRealFFT *forward_real_fft,
    const InverseRealFFT *inverse_real_fft, double *y) {
  fft_complex *x_spectrum = new fft_complex[fft_size];

  for (int i = 0; i < x_length; ++i)
    forward_real_fft->waveform[i] = x[i] / fft_size;
  for (int i = x_length; i < fft_size; ++i)
    forward_real_fft->waveform[i] = 0.0;
  fft_execute(forward_real_fft->forward_fft);
  for (int i = 0; i <= fft_size / 2; ++i) {
    x_spectrum[i][0] = forward_real_fft->spectrum[i][0];
    x_spectrum[i][1] = forward_real_fft->spectrum[i][1];
  }

  for (int i = 0; i < h_length; ++i)
    forward_real_fft->waveform[i] = h[i] / fft_size;
  for (int i = h_length; i < fft_size; ++i)
    forward_real_fft->waveform[i] = 0.0;
  fft_execute(forward_real_fft->forward_fft);

  for (int i = 0; i <= fft_size / 2; ++i) {
    inverse_real_fft->spectrum[i][0] =
      x_spectrum[i][0] * forward_real_fft->spectrum[i][0] -
      x_spectrum[i][1] * forward_real_fft->spectrum[i][1];
    inverse_real_fft->spectrum[i][1] =
      x_spectrum[i][0] * forward_real_fft->spectrum[i][1] +
      x_spectrum[i][1] * forward_real_fft->spectrum[i][0];
  }
  fft_execute(inverse_real_fft->inverse_fft);

  for (int i = 0; i < fft_size; ++i)
    y[i] = inverse_real_fft->waveform[i];

  delete[] x_spectrum;
}

static void FilterForDecimate(const double *x, int x_length, int r, double *y) {
  double a[3], b[2];  // filter Coefficients
  switch (r) {
    case 11:  // fs : 44100 (default)
      a[0] = 2.450743295230728;
      a[1] = -2.06794904601978;
      a[2] = 0.59574774438332101;
      b[0] = 0.0026822508007163792;
      b[1] = 0.0080467524021491377;
      break;
    case 12:  // fs : 48000
      a[0] = 2.4981398605924205;
      a[1] = -2.1368928194784025;
      a[2] = 0.62187513816221485;
      b[0] = 0.0021097275904709001;
      b[1] = 0.0063291827714127002;
      break;
    case 10:
      a[0] = 2.3936475118069387;
      a[1] = -1.9873904075111861;
      a[2] = 0.5658879979027055;
      b[0] = 0.0034818622251927556;
      b[1] = 0.010445586675578267;
      break;
    case 9:
      a[0] = 2.3236003491759578;
      a[1] = -1.8921545617463598;
      a[2] = 0.53148928133729068;
      b[0] = 0.0046331164041389372;
      b[1] = 0.013899349212416812;
      break;
    case 8:  // fs : 32000
      a[0] = 2.2357462340187593;
      a[1] = -1.7780899984041358;
      a[2] = 0.49152555365968692;
      b[0] = 0.0063522763407111993;
      b[1] = 0.019056829022133598;
      break;
    case 7:
      a[0] = 2.1225239019534703;
      a[1] = -1.6395144861046302;
      a[2] = 0.44469707800587366;
      b[0] = 0.0090366882681608418;
      b[1] = 0.027110064804482525;
      break;
    case 6:  // fs : 24000 and 22050
      a[0] = 1.9715352749512141;
      a[1] = -1.4686795689225347;
      a[2] = 0.3893908434965701;
      b[0] = 0.013469181309343825;
      b[1] = 0.040407543928031475;
      break;
    case 5:
      a[0] = 1.7610939654280557;
      a[1] = -1.2554914843859768;
      a[2] = 0.3237186507788215;
      b[0] = 0.021334858522387423;
      b[1] = 0.06400457556716227;
      break;
    case 4:  // fs : 16000
      a[0] = 1.4499664446880227;
      a[1] = -0.98943497080950582;
      a[2] = 0.24578252340690215;
      b[0] = 0.036710750339322612;
      b[1] = 0.11013225101796784;
      break;
    case 3:
      a[0] = 0.95039378983237421;
      a[1] = -0.67429146741526791;
      a[2] = 0.15412211621346475;
      b[0] = 0.071221945171178636;
      b[1] = 0.21366583551353591;
      break;
    case 2:  // fs : 8000
      a[0] = 0.041156734567757189;
      a[1] = -0.42599112459189636;
      a[2] = 0.041037215479961225;
      b[0] = 0.16797464681802227;
      b[1] = 0.50392394045406674;
      break;
    default:
      a[0] = 0.0;
      a[1] = 0.0;
      a[2] = 0.0;
      b[0] = 0.0;
      b[1] = 0.0;
  }

  // Filtering on time domain.
  double w[3] = {0.0, 0.0, 0.0};
  double wt;
  for (int i = 0; i < x_length; ++i) {
    wt = x[i] + a[0] * w[0] + a[1] * w[1] + a[2] * w[2];
    y[i] = b[0] * wt + b[1] * w[0] + b[1] * w[1] + b[0] * w[2];
    w[2] = w[1];
    w[1] = w[0];
    w[0] = wt;
  }
}

void histoc(const double *x, int x_length, const double *edges,
    int edges_length, int *index) {
  int count = 1;

  int i = 0;
  for (; i < edges_length; ++i) {
    index[i] = 1;
    if (edges[i] >= x[0]) break;
  }
  for (; i < edges_length; ++i) {
    if (edges[i] < x[count]) {
      index[i] = count;
    } else {
      index[i--] = count++;
    }
    if (count == x_length) break;
  }
  count--;
  for (i++; i < edges_length; ++i) index[i] = count;
}

void Interp1(const double *x, const double *y, int x_length, const double *xi,
    int xi_length, double *yi) {
  double *h = new double[x_length - 1];
  double *p = new double[xi_length];
  double *s = new double[xi_length];
  int *k = new int[xi_length];

  for (int i = 0; i < x_length - 1; ++i) h[i] = x[i + 1] - x[i];
  for (int i = 0; i < xi_length; ++i) {
    p[i] = i;
    k[i] = 0;
  }

  histoc(x, x_length, xi, xi_length, k);

  for (int i = 0; i < xi_length; ++i)
    s[i] = (xi[i] - x[k[i] - 1]) / h[k[i] - 1];

  for (int i = 0; i < xi_length; ++i)
    yi[i] = y[k[i] - 1] + s[i] * (y[k[i]] - y[k[i] - 1]);

  delete[] k;
  delete[] s;
  delete[] p;
  delete[] h;
}

void decimate(const double *x, int x_length, int r, double *y) {
  const int kNFact = 9;
  double *tmp1 = new double[x_length + kNFact * 2];
  double *tmp2 = new double[x_length + kNFact * 2];

  for (int i = 0; i < kNFact; ++i) tmp1[i] = 2 * x[0] - x[kNFact - i];
  for (int i = kNFact; i < kNFact + x_length; ++i) tmp1[i] = x[i - kNFact];
  for (int i = kNFact + x_length; i < 2 * kNFact + x_length; ++i)
    tmp1[i] = 2 * x[x_length - 1] - x[x_length - 2 - (i - (kNFact + x_length))];

  FilterForDecimate(tmp1, 2 * kNFact + x_length, r, tmp2);
  for (int i = 0; i < 2 * kNFact + x_length; ++i)
    tmp1[i] = tmp2[2 * kNFact + x_length - i - 1];
  FilterForDecimate(tmp1, 2 * kNFact + x_length, r, tmp2);
  for (int i = 0; i < 2 * kNFact + x_length; ++i)
    tmp1[i] = tmp2[2 * kNFact + x_length - i - 1];

  int nout = (x_length - 1) / r + 1;
  int nbeg = r - r * nout + x_length;

  int count = 0;
  for (int i = nbeg; i < x_length + kNFact; i += r)
    y[count++] = tmp1[i + kNFact - 1];

  delete[] tmp1;
  delete[] tmp2;
}
