
#include "piano.h"


//===============================================================================

double pk_88 = 4186.01;	double pk_87 = 3951.07;	double pk_86 = 3729.31;	double pk_85 = 3520.0;	double pk_84 = 3322.44;
double pk_83 = 3135.96;	double pk_82 = 2959.96;	double pk_81 = 2793.83;	double pk_80 = 2637.02;	double pk_79 = 2489.02;
double pk_78 = 2349.32;	double pk_77 = 2217.46;	double pk_76 = 2093.0;	double pk_75 = 1975.53;	double pk_74 = 1864.66;
double pk_73 = 1760.0;	double pk_72 = 1661.22;	double pk_71 = 1567.98;	double pk_70 = 1479.98;	double pk_69 = 1396.91;
double pk_68 = 1318.51;	double pk_67 = 1244.51;	double pk_66 = 1174.66;	double pk_65 = 1108.73;	double pk_64 = 1046.5;
double pk_63 = 987.767;	double pk_62 = 932.328;	double pk_61 = 880.0;	double pk_60 = 830.609;	double pk_59 = 783.991;
double pk_58 = 739.989;	double pk_57 = 698.456;	double pk_56 = 659.255;	double pk_55 = 622.254;	double pk_54 = 587.33;
double pk_53 = 554.365;	double pk_52 = 523.251;	double pk_51 = 493.883;	double pk_50 = 466.164;	double pk_49 = 440.0;
double pk_48 = 415.305;	double pk_47 = 391.995;	double pk_46 = 369.994;	double pk_45 = 349.228;	double pk_44 = 329.628;
double pk_43 = 311.127;	double pk_42 = 293.665;	double pk_41 = 277.183;	double pk_40 = 261.626; double pk_39 = 246.942;
double pk_38 = 233.082;	double pk_37 = 220.0;	double pk_36 = 207.652;	double pk_35 = 195.998;	double pk_34 = 184.997;
double pk_33 = 174.614;	double pk_32 = 164.814;	double pk_31 = 155.563;	double pk_30 = 146.832;	double pk_29 = 138.591;
double pk_28 = 130.813;	double pk_27 = 123.471;	double pk_26 = 116.541;	double pk_25 = 110.0;	double pk_24 = 103.826;
double pk_23 = 97.9989;	double pk_22 = 92.4986;	double pk_21 = 87.3071;	double pk_20 = 82.4069;	double pk_19 = 77.7817;
double pk_18 = 73.4162;	double pk_17 = 69.2957;	double pk_16 = 65.4064;	double pk_15 = 61.7354;	double pk_14 = 58.2705;
double pk_13 = 55.0;	double pk_12 = 51.9131;	double pk_11 = 48.9994;	double pk_10 = 46.2493;	double pk_9	 = 43.6535;
double pk_8	 = 41.2034;	double pk_7	 = 38.8909;	double pk_6	 = 36.7081;	double pk_5	 = 34.6478;	double pk_4	 = 32.7032;
double pk_3	 = 30.8677;	double pk_2	 = 29.1352;	double pk_1	 = 27.5;
/*
	pk(n+1) = factor * pk(n), where factor ~= 1.059463  (1.059462 <= factor <= 1.059464)
			= 27.5 * power(factor, n)
	eg: pk_22 = 27.5 * pow(1.059463, 21) = 27.5 * 3.36357937 = 92.498[43]
			
*/


//===============================================================================


void fade(Voice &v, double vol)
{
	if (!v.size()) return;
	size_t i=0, n=v.size();
	double V=(vol/128.0);
	double ta=(V/double(n)), d;
	int N;
	while (i<n)
	{
		d=((n-i)*ta);
		N=(int)(128.0+(double(v[i]-128)*d));
		v[i]=(VoiceTypeUnit)((N<0)?0:(N>255)?255:N);
		i++;
	}
}

void dblnote(Voice &r1, const Voice &r2)
{
	Voice x, y;
	size_t i, n=r1.size(), m;
	m=(n/2);
	x.clear(); y.clear();
	for (i=0;i<m;i++) x+=r1[i];
	for (;i<n;i++) y+=r1[i];
	y.merge(r2);
	x.blendappend(y);
	r1=x;
}

void tripnote(Voice &r1, const Voice &r2, const Voice &r3)
{
	Voice x, y, z;
	size_t i, n=r1.size(), m;
	m=(n/2);
	x.clear(); y.clear(); z.clear();
	for (i=0;i<m;i++) x+=r1[i];
	for (;i<n;i++) y+=r1[i];
	y.merge(r2);
	for (i=0;i<m;i++) z+=y[i];
	x.blendappend(z);
	z.clear();
	for (;i<n;i++) z+=y[i];
	z.merge(r3);
	x.blendappend(z);
	r1=x;
}

void PKPlay(Voice &v, double pk, double volume, double fadetime)
{
	v.MakeSineWave(pk, volume, fadetime, 8000);
	fade(v, volume);
}

void TwinkleStar(Voice &v)
{
	Voice l1, l2, l3, l4, r1, r2, r3, r4;
	double qt(0.325), ht(0.65), lvol=60.0, rvol=90.0;//, rate=8000.0;
	auto cls=[&]{ l1.clear(); l2.clear(); l3.clear(); l4.clear(); r1.clear(); r2.clear(); r3.clear(); r4.clear();};

	v.clear();
	cls();

	//----------------------------------------------------
		//-----------------------1-1
		PKPlay(r1, pk_40, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_33, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_44, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_35, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v=l1; /* !=! - check copypasta! */
		//-----------------------1-2
		PKPlay(r1, pk_45, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_36, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_44, rvol, ht);
		PKPlay(l2, pk_35, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------1-3
		PKPlay(r1, pk_43, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_34, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_42, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_33, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------1-4
		PKPlay(r1, pk_41, rvol, qt);
		PKPlay(l1, pk_36, lvol, qt); l1.merge(r1);
		PKPlay(l2, pk_37, lvol, qt); l2.merge(r1); l1.blendappend(l2);
		PKPlay(r1, pk_40, rvol, ht);
		PKPlay(l2, pk_35, lvol, qt);
		PKPlay(l3, pk_33, lvol, qt); l2.blendappend(l3); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);

	//----------------------------------------------------
		//-----------------------2-1
		PKPlay(r1, pk_44, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_35, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_43, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_36, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-2
		PKPlay(r1, pk_42, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_37, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_41, rvol, ht);
		PKPlay(l2, pk_37, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-3
		PKPlay(r1, pk_44, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_35, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_43, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_36, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-4
		PKPlay(r1, pk_42, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_37, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_41, rvol, ht);
		PKPlay(l2, pk_37, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);

	//----------------------------------------------------
		//-----------------------3-1
		PKPlay(r1, pk_40, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_33, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_44, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_35, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-2
		PKPlay(r1, pk_45, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_36, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_44, rvol, ht);
		PKPlay(l2, pk_35, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-3
		PKPlay(r1, pk_43, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_34, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_42, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_33, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-4
		PKPlay(r1, pk_41, rvol, qt);
		PKPlay(l1, pk_36, lvol, qt); l1.merge(r1);
		PKPlay(l2, pk_37, lvol, qt); l2.merge(r1); l1.blendappend(l2);
		PKPlay(r1, pk_40, rvol, ht+qt);
		PKPlay(l2, pk_33, lvol, ht+qt); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
}

void TwinkleStar_Treble(Voice &v)
{
	Voice l1, l2, l3, l4, r1, r2, r3, r4;
	double qt(0.325), ht(0.65), lvol=60.0, rvol=90.0;//, rate=8000.0;
	auto cls=[&]{ l1.clear(); l2.clear(); l3.clear(); l4.clear(); r1.clear(); r2.clear(); r3.clear(); r4.clear();};

	v.clear();
	cls();

	//----------------------------------------------------
		//-----------------------1-1
		PKPlay(r1, pk_47, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_40, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_51, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_42, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v=l1; /* !=! - check copypasta! */
		//-----------------------1-2
		PKPlay(r1, pk_52, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_43, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_51, rvol, ht);
		PKPlay(l2, pk_42, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------1-3
		PKPlay(r1, pk_50, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_41, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_49, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_40, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------1-4
		PKPlay(r1, pk_48, rvol, qt);
		PKPlay(l1, pk_43, lvol, qt); l1.merge(r1);
		PKPlay(l2, pk_44, lvol, qt); l2.merge(r1); l1.blendappend(l2);
		PKPlay(r1, pk_47, rvol, ht);
		PKPlay(l2, pk_42, lvol, qt);
		PKPlay(l3, pk_40, lvol, qt); l2.blendappend(l3); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);

	//----------------------------------------------------
		//-----------------------2-1
		PKPlay(r1, pk_51, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_42, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_50, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_43, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-2
		PKPlay(r1, pk_49, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_44, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_48, rvol, ht);
		PKPlay(l2, pk_44, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-3
		PKPlay(r1, pk_51, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_42, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_50, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_43, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------2-4
		PKPlay(r1, pk_49, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_44, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_48, rvol, ht);
		PKPlay(l2, pk_44, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);

	//----------------------------------------------------
		//-----------------------3-1
		PKPlay(r1, pk_47, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_40, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_51, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_42, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-2
		PKPlay(r1, pk_52, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_43, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_51, rvol, ht);
		PKPlay(l2, pk_42, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-3
		PKPlay(r1, pk_50, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l1, pk_41, lvol, ht); l1.merge(r1);
		PKPlay(r1, pk_49, rvol, qt); r2=r1; r1.blendappend(r2);
		PKPlay(l2, pk_40, lvol, ht); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
		//-----------------------3-4
		PKPlay(r1, pk_48, rvol, qt);
		PKPlay(l1, pk_43, lvol, qt); l1.merge(r1);
		PKPlay(l2, pk_44, lvol, qt); l2.merge(r1); l1.blendappend(l2);
		PKPlay(r1, pk_47, rvol, ht+qt);
		PKPlay(l2, pk_40, lvol, ht+qt); l2.merge(r1); l1.blendappend(l2);
		v.blendappend(l1);
}



void AmaGrace(Voice &v)
{
	Voice l1, l2, l3, l4, r1, r2, r3, r4;
	double t13(0.4), t23(0.8), t33(1.2), lvol=60.0, rvol=90.0;//, rate=8000.0;
	auto cls=[&]{ l1.clear(); l2.clear(); l3.clear(); l4.clear(); r1.clear(); r2.clear(); r3.clear(); r4.clear();};

	v.clear();
	cls();

	//----------------------------------------------------
		//-----------------------1-0
		PKPlay(r1, pk_41, rvol, t13);
		PKPlay(r2, pk_39, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t13); r1.merge(l1);
		v=r1; /* !=! - check copypasta! */
		//-----------------------1-1
		PKPlay(r1, pk_44, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_30, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_46, rvol, t13);
		PKPlay(r2, pk_45, rvol, t13);
		PKPlay(r3, pk_44, rvol, t13); tripnote(r1, r2, r3);
		PKPlay(l1, pk_30, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------1-2
		PKPlay(r1, pk_46, rvol, t23);
		PKPlay(r2, pk_41, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_45, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_36, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------1-3
		PKPlay(r1, pk_44, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_35, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_42, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_33, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------1-4
		PKPlay(r1, pk_41, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_30, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_41, rvol, t13);
		PKPlay(r2, pk_39, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		
	//----------------------------------------------------
		//-----------------------2-1
		PKPlay(r1, pk_44, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_30, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_46, rvol, t13);
		PKPlay(r2, pk_45, rvol, t13);
		PKPlay(r3, pk_44, rvol, t13); tripnote(r1, r2, r3);
		PKPlay(l1, pk_30, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------2-2
		PKPlay(r1, pk_46, rvol, t23);
		PKPlay(r2, pk_41, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_45, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_36, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_33, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------2-3
		PKPlay(r1, pk_48, rvol, t33);
		PKPlay(r2, pk_41, rvol, t33); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t33); r1.merge(l1);
		PKPlay(l1, pk_32, lvol, t33); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------2-4
		PKPlay(r1, pk_48, rvol, t23);
		PKPlay(r2, pk_41, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_36, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_31, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_46, rvol, t13);
		PKPlay(r2, pk_41, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_34, lvol, t13); r1.merge(l1);
		v.blendappend(r1);

	//----------------------------------------------------
		//-----------------------3-1
		PKPlay(r1, pk_48, rvol, t13);
		PKPlay(r2, pk_41, rvol, t13); r1.merge(r2);
		PKPlay(r2, pk_46, rvol, t13); r1.blendappend(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_30, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_48, rvol, t13);
		PKPlay(r2, pk_41, rvol, t13); r1.merge(r2);
		PKPlay(r2, pk_46, rvol, t13); dblnote(r1, r2);
		PKPlay(l1, pk_39, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------3-2
		PKPlay(r1, pk_44, rvol, t23);
		PKPlay(r2, pk_41, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_32, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_41, rvol, t13);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_32, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------3-3
		PKPlay(r1, pk_42, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(r2, pk_44, rvol, t13); r1.blendappend(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_33, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_44, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(r2, pk_42, rvol, t13); dblnote(r1, r2);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_35, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------3-4
		PKPlay(r1, pk_41, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_30, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_41, rvol, t13);
		PKPlay(l1, pk_36, lvol, t13); r1.merge(l1);
		v.blendappend(r1);

	//----------------------------------------------------
		//-----------------------4-1
		PKPlay(r1, pk_44, rvol, t23);
		PKPlay(r2, pk_39, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_35, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_46, rvol, t13);
		PKPlay(r2, pk_45, rvol, t13);
		PKPlay(r3, pk_44, rvol, t13); tripnote(r1, r2, r3);
		PKPlay(l1, pk_37, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_35, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------4-2
		PKPlay(r1, pk_46, rvol, t23);
		PKPlay(r2, pk_41, rvol, t23); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t23); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t23); r1.merge(l1);
		v.blendappend(r1);
		PKPlay(r1, pk_45, rvol, t13);
		PKPlay(r2, pk_40, rvol, t13); r1.merge(r2);
		PKPlay(l1, pk_36, lvol, t13); r1.merge(l1);
		PKPlay(l1, pk_34, lvol, t13); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------4-3
		PKPlay(r1, pk_44, rvol, t33);
		PKPlay(r2, pk_42, rvol, t33); r1.merge(r2);
		PKPlay(l1, pk_37, lvol, t33); r1.merge(l1);
		PKPlay(l1, pk_33, lvol, t33); r1.merge(l1);
		v.blendappend(r1);
		//-----------------------4-4
		PKPlay(r1, pk_44, rvol, t33+t13);
		PKPlay(r2, pk_39, rvol, t33+t13); r1.merge(r2);
		PKPlay(l1, pk_34, lvol, t33+t13); r1.merge(l1);
		PKPlay(l1, pk_30, lvol, t33+t13); r1.merge(l1);
		v.blendappend(r1);

}

void OwnCompo(Voice &v)
{
	Voice l1, l2, l3, l4, r1, r2, r3, r4;
	double qt(0.325), ht(0.65), ft(1.3), rvol=90.0;//, rate=8000.0;//, lvol=45.0;
	auto cls=[&]{ l1.clear(); l2.clear(); l3.clear(); l4.clear(); r1.clear(); r2.clear(); r3.clear(); r4.clear();};

	v.clear();
	cls();

	//---------------------------------------------------- //this is fun!
		//-----------------------1-1
		PKPlay(r1, pk_43, rvol, qt);
		PKPlay(r2, pk_45, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_47, rvol, ht); r1.blendappend(r2);
		v=r1; /* !=! - check copypasta! */
		//-----------------------1-2
		PKPlay(r1, pk_44, rvol, qt);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_48, rvol, ht); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------1-3
		PKPlay(r1, pk_46, rvol, qt);
		PKPlay(r2, pk_44, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_44, rvol, qt); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------1-4
		PKPlay(r1, pk_42, rvol, ht); r2=r1; r1.blendappend(r2);
		v.blendappend(r1);

	//----------------------------------------------------
		//-----------------------2-1
		PKPlay(r1, pk_42, rvol, qt);
		PKPlay(r2, pk_44, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_46, rvol, ht); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-2
		PKPlay(r1, pk_47, rvol, qt);
		PKPlay(r2, pk_44, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_41, rvol, qt); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-3
		PKPlay(r1, pk_44, rvol, qt);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_48, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_50, rvol, qt); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-4
		PKPlay(r1, pk_45, rvol, ht); r2=r1; r1.blendappend(r2);
		v.blendappend(r1);

	//----------------------------------------------------
		//-----------------------2-1
		PKPlay(r1, pk_46, rvol, qt);
		PKPlay(r2, pk_47, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_48, rvol, ht); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-2
		PKPlay(r1, pk_44, rvol, qt);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_45, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_47, rvol, qt); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-3
		PKPlay(r1, pk_48, rvol, qt);
		PKPlay(r2, pk_46, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_50, rvol, qt); r1.blendappend(r2);
		PKPlay(r2, pk_47, rvol, qt); r1.blendappend(r2);
		v.blendappend(r1);
		//-----------------------2-4
		PKPlay(r1, pk_48, rvol, ft+qt);
		v.blendappend(r1);

}


void StepAllKeys(Voice &v)
{
	Voice k, r;
	double qt(0.1), vol(60.0);
	
	v.clear();
	
	//PKPlay(k, pk_88, vol, qt); v=k; // = 4186.01, vol, qt); v.blendappend(k); //last key on right-hand side //can't reach in voice mode
	//PKPlay(k, pk_87, vol, qt); v=k; // = 3951.07; --- can't reach this either
	PKPlay(k, pk_86, vol, qt); r=v=k; // = 3729.31;
	PKPlay(k, pk_85, vol, qt); v.blendappend(k); k.blendappend(r); r=k;
	PKPlay(k, pk_84, vol, qt); v.blendappend(k); k.blendappend(r); r=k;
	PKPlay(k, pk_83, vol, qt); v.blendappend(k); k.blendappend(r); r=k;
	PKPlay(k, pk_82, vol, qt); v.blendappend(k); k.blendappend(r); r=k;
	PKPlay(k, pk_81, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2793.83;
	PKPlay(k, pk_80, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2637.02;
	PKPlay(k, pk_79, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2489.02;
	PKPlay(k, pk_78, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2349.32;
	PKPlay(k, pk_77, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2217.46;
	PKPlay(k, pk_76, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 2093.0;
	PKPlay(k, pk_75, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1975.53;
	PKPlay(k, pk_74, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1864.66;
	PKPlay(k, pk_73, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1760.0;
	PKPlay(k, pk_72, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1661.22;
	PKPlay(k, pk_71, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1567.98;
	PKPlay(k, pk_70, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1479.98;
	PKPlay(k, pk_69, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1396.91;
	PKPlay(k, pk_68, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1318.51;
	PKPlay(k, pk_67, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1244.51;
	PKPlay(k, pk_66, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1174.66;
	PKPlay(k, pk_65, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1108.73;
	PKPlay(k, pk_64, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 1046.5;
	PKPlay(k, pk_63, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 987.767;
	PKPlay(k, pk_62, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 932.328;
	PKPlay(k, pk_61, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 880.0;
	PKPlay(k, pk_60, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 830.609;
	PKPlay(k, pk_59, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 783.991;
	PKPlay(k, pk_58, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 739.989;
	PKPlay(k, pk_57, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 698.456;
	PKPlay(k, pk_56, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 659.255;
	PKPlay(k, pk_55, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 622.254;
	PKPlay(k, pk_54, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 587.33;
	PKPlay(k, pk_53, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 554.365;
	PKPlay(k, pk_52, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 523.251;
	PKPlay(k, pk_51, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 493.883;
	PKPlay(k, pk_50, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 466.164;
	PKPlay(k, pk_49, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 440.0;
	PKPlay(k, pk_48, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 415.305;
	PKPlay(k, pk_47, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 391.995;
	PKPlay(k, pk_46, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 369.994;
	PKPlay(k, pk_45, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 349.228;
	PKPlay(k, pk_44, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 329.628;
	PKPlay(k, pk_43, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 311.127;
	PKPlay(k, pk_42, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 293.665;
	PKPlay(k, pk_41, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 277.183;
	PKPlay(k, pk_40, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 261.626, vol, qt); v.blendappend(k); k.blendappend(r); r=k; //middle C
	PKPlay(k, pk_39, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 246.942;
	PKPlay(k, pk_38, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 233.082;
	PKPlay(k, pk_37, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 220.0;
	PKPlay(k, pk_36, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 207.652;
	PKPlay(k, pk_35, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 195.998;
	PKPlay(k, pk_34, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 184.997;
	PKPlay(k, pk_33, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 174.614;
	PKPlay(k, pk_32, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 164.814;
	PKPlay(k, pk_31, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 155.563;
	PKPlay(k, pk_30, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 146.832;
	PKPlay(k, pk_29, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 138.591;
	PKPlay(k, pk_28, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 130.813;
	PKPlay(k, pk_27, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 123.471;
	PKPlay(k, pk_26, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 116.541;
	PKPlay(k, pk_25, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 110.0;
	PKPlay(k, pk_24, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 103.826;
	PKPlay(k, pk_23, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 97.9989;
	PKPlay(k, pk_22, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 92.4986;
	PKPlay(k, pk_21, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 87.3071;
	PKPlay(k, pk_20, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 82.4069;
	PKPlay(k, pk_19, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 77.7817;
	PKPlay(k, pk_18, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 73.4162;
	PKPlay(k, pk_17, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 69.2957;
	PKPlay(k, pk_16, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 65.4064;
	PKPlay(k, pk_15, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 61.7354;
	PKPlay(k, pk_14, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 58.2705;
	PKPlay(k, pk_13, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 55.0;
	PKPlay(k, pk_12, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 51.9131;
	PKPlay(k, pk_11, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 48.9994;
	PKPlay(k, pk_10, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 46.2493;
	PKPlay(k, pk_9	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 43.6535;
	PKPlay(k, pk_8	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 41.2034;
	PKPlay(k, pk_7	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 38.8909;
	PKPlay(k, pk_6	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 36.7081;
	PKPlay(k, pk_5	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 34.6478;
	PKPlay(k, pk_4	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 32.7032;
	PKPlay(k, pk_3	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 30.8677;
	PKPlay(k, pk_2	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 29.1352;
	PKPlay(k, pk_1	, vol, qt); v.blendappend(k); k.blendappend(r); r=k; // = 27.5, vol, qt); v.blendappend(k); k.blendappend(r); r=k; //first key on left-hand side
	
	v.blendappend(r);
}


void FourChords(Voice &v)
{
	Voice l1, l2, l3, l4, r1, r2, r3, r4;
	double t, qt(0.325), rvol=90.0;//, rate=8000.0, ht(0.65), ft(1.3), lvol=45.0;
	auto cls=[&]{ l1.clear(); l2.clear(); l3.clear(); l4.clear(); r1.clear(); r2.clear(); r3.clear(); r4.clear();};

	v.clear();
	cls();

	t=qt;

	//----------------------------------------------------
		//-----------------------1
		PKPlay(r1, pk_33, rvol, t);
		//PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v=r1; /* !=! - check copypasta! <----------------------------- slow learner, are you?*/
		//-----------------------2
		PKPlay(r1, pk_33, rvol, t);
		//PKPlay(r2, pk_36, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_38, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------3
		PKPlay(r1, pk_32, rvol, t);
		//PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------4
		PKPlay(r1, pk_32, rvol, t);
		//PKPlay(r2, pk_34, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);

///rep...2
		//-----------------------1
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------2
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_36, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_38, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------3
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------4
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_34, rvol, t); r1.merge(r2);
		//PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);

///rep...3
		//-----------------------1
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------2
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_36, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_38, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------3
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------4
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_34, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);

///rep...4
		//-----------------------1
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------2
		PKPlay(r1, pk_33, rvol, t);
		PKPlay(r2, pk_36, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_38, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------3
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_35, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);
		//-----------------------4
		PKPlay(r1, pk_32, rvol, t);
		PKPlay(r2, pk_34, rvol, t); r1.merge(r2);
		PKPlay(r2, pk_37, rvol, t); r1.merge(r2);
		v.blendappend(r1);


}
