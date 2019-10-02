
#include "vanautil.h"
#include <cmath>


void dummy() {}




//bool GetVoiceStats(const Voice &v, VoiceStats &ss)
//{
/*
	if (us.size()<3) return false;
	size_t i;
	uint8_t p=us[0];

	ss.nInterval=us.size();
	ss.nMaxima=ss.nMinima=0;
	ss.damphigh=ss.damplow=128.0;
	ss.daverage=ss.dmean=ss.dfreq=0.0;
	
	for (i=1;i<(us.size()-1);i++)
	{
		ss.daverage+=us[i];
		if (us[i]>ss.damphigh) ss.damphigh=us[i];
		if (us[i]<ss.damplow) ss.damplow=us[i];
		if ((us[i]>p)&&(us[i]>us[i+1])) { ss.nMaxima++; p=us[i]; }
		if ((us[i]<p)&&(us[i]<us[i+1])) { ss.nMinima++; p=us[i]; }
	}
	ss.daverage/=(ss.nInterval-2); ss.daverage-=128;
	ss.damphigh-=128.0; ss.damplow-=128.0;
	ss.dmean=(ss.damphigh+ss.damplow)/2.0;
	ss.dfreq=(0.5*(ss.nMaxima+ss.nMinima));
	ss.dwavelen=(ss.nInterval/ss.dfreq);
*/
//	return true;
//}


//bool HasVoice(const Voice &v)
//{
//	size_t i=0, m=data.size(), hl=0; //hl=high/low-count
//	char u;
//	while ((i<m)&&(hl<SCAN_DISTANCE)) { u=data[i++]; if ((u<(-NOISE_DELTA))||(u>NOISE_DELTA)) hl++; }
//	return (hl>SCAN_DISTANCE);
//}


/*
    A sample rate of 44.1 kHz is 44,100 samples/second
    A 1 kHz wave is 1,000 cycles/second
    The sine function has a cycle length of 2*pi 

So. You need to scale your sine function such that 1,000 sine wave cycles would fit into 44,100 samples. Here's a rough look at how you'd derive code to fill the buffer:

for(i = 0; i < 10000; i++)
{
  // i is the sample index

  // Straight sine function means one cycle every 2*pi samples:
  // buffer[i] = sin(i); 

  // Multiply by 2*pi--now it's one cycle per sample:
  // buffer[i] = sin((2 * pi) * i); 

  // Multiply by 1,000 samples per second--now it's 1,000 cycles per second:
  // buffer[i] = sin(1000 * (2 * pi) * i);

  // Divide by 44,100 samples per second--now it's 1,000 cycles per 44,100
  // samples, which is just what we needed:
  buffer[i] = sin(1000 * (2 * pi) * i / 44100);
}


# include < stdio.h >
# include < stdlib.h >
# include < math.h >
# define n_points 32 //256 is ideal for microcontrollers or highers if youre computing complex sinusoids
int main ()
{
    float pi = 3.141592;
    float w ;    // ψ
    float yi ;
    float phase;
    int sign_samp;
    int sin_data[n_points];  // sine LUT Array
    int i;
    w= 2*pi;
    w= w/n_points;
    for (i = 0; i <= n_points; i++)
    {
        yi= 2047*sin(phase);
        phase=phase+w;
        sign_samp=2047+yi;     // dc offset translated for a 12 bit DAC
        //sign_samp+= b;         // Add adc value; Keep it at zero for pure sine
        sin_data[i]=sign_samp; // write value into array
        / *
        * /
    }
    for (i = 0; i <= n_points; i++)
    {
        int k=sin_data[i];
        printf ("sine is %d\n", k);
        printf ("i is ... %d\n", i);
    }
    float x;
    printf ("Enter a number ... \n");
    scanf ("%f", &x); // keeps the debugging console on codeblocks open ;)
    return 0;
}

*/

//void ClearNoise(Voice &v)
//{
//	data[0]=128;
//	for (size_t i=2;i<(data.size()-2);i++)
//	{
//		uint8_t n=((data[i-2]+data[i-1]+data[i]+data[i+1]+data[i+2])/5); ///muffles sound - like speaking through cotton
//		data[i]=n;
//	}

/*

	for (size_t i=0;i<data.size();i++)
	{
		uint8_t u=data[i], t;
		t=(u<(128-NOISE_DELTA))?(u+NOISE_DELTA):(u>(128+NOISE_DELTA))?(u-NOISE_DELTA):128;
		data[i]=t;
	}
*/
//}

//bool IsNoiseTrend(const Voice &v, size_t pos, int pf, size_t dist)
//{
/*
	auto trend=[](const RawVoice &u, size_t f, size_t t)
				{
					size_t i=0, a=0;
					for (i=f;i<=t;i++) a+=(u[i]>128)?(u[i]-128):(128-u[i]);
					a/=(t-f+1);
					//return ((a<>=(128-NOISE_DELTA))&&(a<=(128+NOISE_DELTA)));
					return (a<=NOISE_DELTA);
				};
	size_t n=((pos>dist)?(pos-dist):0);
	size_t m=(((pos+dist)<=us.size())?(pos+dist):us.size());
	if (pf==PAST_TREND) return trend(us, n, pos);
	if (pf==FUTURE_TREND) return trend(us, pos, m);
	if (pf==LOCAL_TREND) return (trend(us, n, pos)&&trend(us, pos, m));
*/
//	return false;
//}

//void ClearNoiseTrend(Voice &v, int pf)
//{
//	for (size_t i=0;i<data.size();i++) if (IsNoiseTrend(data, i, pf, SCAN_DISTANCE)) data[i]=128;
//}

//void LowPassFilter(Voice &v)
//{
/*
	static uint8_t p=data[0];
	uint16_t a;
	for (size_t i=1;i<data.size();i++)
	{
		a=(data[i]+p)/2;
		p=data[i];
		data[i]=(uint8_t)a;
	}
*/
//}

//void LowPassFilter2(Voice &v)
//{
/*
	static uint8_t p=data[0];
	uint16_t a;
	for (size_t i=1;i<(data.size()-1);i++)
	{
		a=(data[i]+p+data[i+1])/3;
		p=data[i];
		data[i]=(uint8_t)a;
	}
*/
//}

//-----------------
//can use s'thing like this to find noise thresholds...
//-----------------
//	uint16_t minval=0x7f, maxval=0x7f;
//	double average=0.0;
//	for (i=0;i<data.size();i++)
//	{
//		average+=data[i];
//		if (data[i]<minval) minval=data[i];
//		if (data[i]>maxval) maxval=data[i];
//	}
//	average/=double(data.size());
//	fout("\n-----\nave = %E\nmin = %d\nmax = %d\n-----\n", average, minval, maxval);
//	RLEnc(data,rle);
//	xout(rle);
//	return 0;

