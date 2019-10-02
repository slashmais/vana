#ifndef _voice_h_
#define _voice_h_

//#include <vector>
//#include <vioctrl/vioctrl.h>
//#include <string>
//#include <alsa/asoundlib.h>


void dummy();


/*
//should be a calculated value...
#define NOISE_DELTA (char)5

//minimum byte-count of not-noise data...
#define SCAN_DISTANCE (size_t)200

bool GetVoiceStats(const Voice &v, VoiceStats &ss);
enum { PAST_TREND=1, FUTURE_TREND=2, LOCAL_TREND=4, };
bool IsNoiseTrend(const Voice &v, size_t pos, int pf, size_t dist);
bool HasVoice(const Voice &v);
void ClearNoise(Voice &v);
void ClearNoiseTrend(Voice &v, int pf);
void LowPassFilter(Voice &v);
void LowPassFilter2(Voice &v);
*/


//-----------------------------------------------------------------------------
const double epsilon=1.0e-11L; //1.0e-15L;
auto is_zero=[](double d)->bool{ return (d<0.0)?(d>=-epsilon):(d<=epsilon); };
auto not_zero=[](double d)->bool{ return !is_zero(d); };
auto is_equal=[](double l, double r)->bool{ return is_zero(l-r); };
auto not_equal=[](double l, double r)->bool{ return !is_equal(l,r); };
auto check_zero=[](double d){ return (is_zero(d))?0.0:d; }; //get rid of "-0.0" rubbish
auto d_int=[&](double d, bool nearest=true)->int{ return (nearest)?(int)(check_zero((d>=0.0)?(d+0.5):(d-0.5))):(int)d; };

//-----------------------------------------------------------------------------
template<typename T> struct RunStats
{
private:
	double runavg, runmin, runmax, runvar, t;
	size_t runcount;
public:
	RunStats() : runcount(0) { runavg=runmin=runmax=runvar=0.0; }
	void Add(T newnum)
	{
		double C(runcount), N(newnum);
		if (!runcount) { runcount=1; runavg=runmin=runmax=N; runvar=0.0; }
		else
		{
			if (runmin>N) runmin=N; else if (runmax<N) runmax=N;
			//runavg = (runavg*C + N)/(C+1.0);
			runavg = ((runavg/((1.0/C)+1.0)) + (N/(C+1.0)));
			t=(newnum-runavg); t*=t;
			runvar*=runcount; runvar+=t;
			runcount++;
			runvar/=runcount;
		}
	}
	size_t Count() { return runcount; }
	void Reset() { runcount=0; runavg=runmin=runmax=0.0; }
	T Mean() { return (T)runavg; }
	T Min() { return (T)runmin; }
	T Max() { return (T)runmax; }
	T Median() { return (T)((runmin+runmax)/2.0); }
	T Deviation() { return (T)sqrt(runvar); }
	double RealMean() { return runavg; }
	double RealMedian() { return ((runmin+runmax)/2.0); }
	double RealDeviation() { return sqrt(runvar); }
	double Variance() { return runvar; }
};

/*
RunStats:
	[a is average, i is count, n is new number]
	new_a = (a*i + n) / (i+1)			...[(a*i) can overflow Type]
	  = (a*i)/(i+1) + n/(i+1)		...(1)
	inverting (a*i)/(i+1) from (1):
		(i+1)/(a*i) = i/(a*i) + 1/(a*i)
					= 1/a + 1/(a*i)
					= (1/a)*(1 + 1/i)
					= (1 + 1/i)/a		...(2)
		inverting (2):
			a/(1 + 1/i)						...(3)
		substituting (3) back into (1):
	new_a = a/(1 + 1/i) + n/(i+1)		...[with no danger of overflow]
*/


#endif
