
#include "vana.h"
#include "fft.h"

const double PI=double(3.14159265358979323846264338328);
const double PI2=double(PI*2);

void fft(VCD &vcd)
{
	size_t N=vcd.size(), k, n;
	double thetaT=(PI/N);
	ComplexDbl phiT = ComplexDbl(cos(thetaT), sin(thetaT)), T;
	
	k=N;
	while (k > 1)
	{
		n=k;
		k>>=1;
		phiT*=phiT;
		T=1.0L;
		for (size_t l=0; l<k; l++)
		{
			for (size_t a=l; a<N; a+=n)
			{
				size_t b=a+k;
				ComplexDbl t=(vcd[a]-vcd[b]);
				vcd[a]+=vcd[b];
				vcd[b]=(t*T);
			}
			T*=phiT;
		}
	}

	size_t m=(size_t)log2(N);
	for (size_t a=0; a<N; a++)
	{
		size_t b=a;
		// Reverse bits
		b=(((b&0xaaaaaaaa)>>1)|((b&0x55555555)<<1));
		b=(((b&0xcccccccc)>>2)|((b&0x33333333)<<2));
		b=(((b&0xf0f0f0f0)>>4)|((b&0x0f0f0f0f)<<4));
		b=(((b&0xff00ff00)>>8)|((b&0x00ff00ff)<<8));
		b=((b>>16)|(b<<16))>>(32-m);
		if (b>a) { ComplexDbl t=vcd[a]; vcd[a]=vcd[b]; vcd[b]=t; }
	}
}
 

void ifft(VCD &vcd) //inverse
{
	for (auto &c:vcd) c.imag(-c.imag());
    fft(vcd);
	for (auto &c:vcd) { c.imag(-c.imag()); c/=vcd.size(); }//is the scaling "/=" this necessary?
}

//===============================================================================

/*
		 N-1
		 ---
   X  =  \        -((2πi)/N)nk		=> X-kay = SUM(for n=0 to N-1) { x-enn times e to-the-power(minus(2pi(imaginary) divided by N) times (n times kay))
	k    /    x  e
		 ---   n
		 n=0


	X[0],...,[N−1] = ditfft2(x, N, s):					DFT of (x0, xs, x2s, ..., x(N-1)s):
    if N = 1 then
        X[0] = x0                                       trivial size-1 DFT base case
    else
        //if N is odd, chop last or add 0 or dup last or add avg of last two or ...
        X[0],...,[N/2−1] = ditfft2(x, N/2, 2s)          DFT of (x0, x2s, x4s, ...)
        X[N/2],...,[N−1] = ditfft2(x+s, N/2, 2s)        DFT of (xs, xs+2s, xs+4s, ...)
        for k = 0 to N/2−1                              combine DFTs of two halves into full DFT:
            t = X[k]
            X[k] = t + exp(−2πi k/N) X[k+N/2]
            X[k+N/2] = t − exp(−2πi k/N) X[k+N/2]
        endfor
    endif
*/



