
#include "vana.h"
#include "fft.h"
#include <cmath>
#include <vector>
///#include "maisutils.h"
#include <utilfuncs/utilfuncs.h>
#include "vanautil.h"


int npaint;

//===============================================================================

SpectrumPanel::SGramCtrl::SGramCtrl(SpectrumPanel &par) : SP(par)
{
	SetRect(0,0,100,272);
	AddFrame(HS);  HS.SetLine(10); HS.Set(0,0,0); HS.WhenScroll << [&]{ Refresh(); SP.Refresh(); };
	AddFrame(VS); VS.SetLine(10); VS.Set(0,0,0); VS.WhenScroll << [&]{ Refresh(); SP.Refresh(); };

	//vld.clear();
	ldpt.x=99999;
	mvpt.x=mvpt.y=-10;
}

void SpectrumPanel::SGramCtrl::Layout()
{
	Size sz=GetSize();
	int ytop=(SGPic.GetHeight()-sz.cy); if (ytop<0) ytop=0;
	VS.Set(ytop, sz.cy, SGPic.GetHeight());
	HS.Set(0, sz.cx, SGPic.GetWidth());
}

void SpectrumPanel::SGramCtrl::ShowMagnitudes(Draw &drw, int W, int H)
{
	if ((ffpt.x>=0)&&(ffpt.x<(int)SP.vvcd.size()))
	{
		Color pink=Color(244, 7, 255);
		//Color dkpink=Color(61, 2, 64);
		int c,Y=SP.vvcd[0].size();

		for (int i=0;i<Y;i++)
		{
			c=(int)(Y*SP.vvcd[ffpt.x][i].imag());
			drw.DrawLine(ffpt.x-HS, i-VS, ffpt.x+c-HS, i-VS, 1, pink);
		}
		drw.DrawLine(ffpt.x-HS,0,ffpt.x-HS,Y,PEN_DOT,pink);
	}
}

void SpectrumPanel::SGramCtrl::Paint(Draw &drw)
{
	Size sz=GetSize();
	drw.DrawRect(sz, White());
	
	drw.DrawImage(-HS, -VS, SGPic);

	drw.DrawLine(0, mvpt.y, mvpt.x, mvpt.y, PEN_DOT, Color(244, 7, 255)); //Color(64,0,0));
	drw.DrawLine(mvpt.x, 0, mvpt.x, sz.cy, PEN_DASH, Red());
	
	if (ffpt.x>=0) ShowMagnitudes(drw, sz.cx, sz.cy);

}

void SpectrumPanel::SGramCtrl::ShowSpectrum()
{
	auto invert_rgba=[](RGBA &c){ c.r=(255-c.r); c.g=(255-c.g); c.b=(255-c.b); };
	
	SGPic.Clear();
	if (SP.vvcd.size())
	{
		size_t x, y, X=SP.vvcd.size(), Y=SP.vvcd[0].size();
		int c;
		ImageBuffer ib(X, Y);
		RGBA cola;

		for (x=0;x<X;x++)
		{
			for (y=0;y<Y;y++)
			{
				c=(int)(Y*SP.vvcd[x][y].imag()*SP.adjust_intensity);
				cola=RGBAZero();
				cola.r=cola.g=cola.b=c; cola.a=255;
				if (SP.bInvertColors) invert_rgba(cola);
				ib[y][x]=cola;
			}
		}
		Premultiply(ib);
		SGPic=ib;
	}

	Size sz=GetSize();
	int ytop=(SGPic.GetHeight()-sz.cy); if (ytop<0) ytop=0;
	VS.Set(ytop, sz.cy, SGPic.GetHeight());
	HS.Set(0, sz.cx, SGPic.GetWidth()); //show bottom-left of picture
	Refresh();
}

void SpectrumPanel::SGramCtrl::MouseEnter(Point p, dword keyflags) { }
void SpectrumPanel::SGramCtrl::MouseLeave() { mvpt.x=mvpt.y=-10; Refresh(); SP.Refresh(); }
void SpectrumPanel::SGramCtrl::MouseWheel(Point p, int zdelta, dword keyflags) { VS.Wheel(zdelta); }

void SpectrumPanel::SGramCtrl::MouseMove(Point p, dword keyflags)
{
	mvpt=p;
	Refresh();
	SP.Refresh();
}

void SpectrumPanel::SGramCtrl::LeftDown(Point p, dword keyflags)
{
	ldpt=p;
	ldpt+=Point(HS,VS);
}

void SpectrumPanel::SGramCtrl::LeftUp(Point P, dword keyflags)
{
	Point p=P;
	p+=Point(HS,VS);
	int d=(ldpt.x-p.x);

	//auto check_remove=[&](const Point &p) { bool b=false;
	//										auto it=vld.begin();
	//										while (!b&&(it!=vld.end())) { int d=(it->x-p.x); if (!(b=((d>=(-2))&&(d<=2)))) it++; }
	//										if (b) vld.erase(it);
	//										return b; };
	////if ((d>=(-2))&&(d<=2)) { if (!check_remove(ldpt)) vld.push_back(ldpt); }
	//if ((d>=(-2))&&(d<=2)) // { if (!check_remove(ldpt)) {vld.clear();  vld.push_back(ldpt); }}
	
	if ((d>=(-5))&&(d<=5)) { d=(ffpt.x-ldpt.x); if ((d>=(-5))&&(d<=5)) ffpt.x=(-1); else ffpt=ldpt; }
	
	ldpt.x=99999;
	Refresh();
	
	if (WhenLU) WhenLU();
}


//===============================================================================

SpectrumPanel::SpectrumPanel(VANA *par) : Panel(par) , SG(*this)
{
	nWinSize=BASE_WIN_SIZE;
	nStep=BASE_STEP_SIZE;
	adjust_intensity=1.0;
	bInvertColors=false;
	bZoom=false;

	Add(SG.AddFrame(BlackFrame()).HSizePosZ(65).TopPos(0, 274));
		SG.Enable(false);
		SG.WhenLU = THISBACK(FillSpectralSection);

	Add(lb.HSizePosZ().TopPos(275, 30));
		Add(btnSpectrum.SetLabel("Show Spectrum").LeftPos(5, 120).TopPos(280, 20));
			btnSpectrum.WhenPush = THISBACK(ShowSpectrum);
		Add(lSA.SetLabel("Intensity:").LeftPos(130, 60).TopPos(280, 20));
			Add(EdSA.LeftPos(190,50).TopPos(280, 20));
				EdSA.MinMax(1.0, 99.9); EdSA.SetData(adjust_intensity); EdSA.SetInc(0.1);
				EdSA.WhenAction << [&]{ adjust_intensity=~EdSA; SG.ShowSpectrum(); };
		Add(optCols.SetLabel("Invert colors").LeftPos(250,100).TopPos(280, 20));
			optCols.Set(1);
			optCols.WhenPush << [&]{ bInvertColors=(~optCols==0)?false:true; SG.ShowSpectrum(); };
		Add(btnZoom.SetLabel("Double size").LeftPos(360,90).TopPos(280, 20));
			btnZoom.WhenPush << [&]{	bZoom=!bZoom;
										if (bZoom) { nWinSize=BASE_WIN_SIZE*2; nStep=BASE_STEP_SIZE/4; }
										else { nWinSize=BASE_WIN_SIZE; nStep=BASE_STEP_SIZE; }
										btnZoom.SetLabel((bZoom)?"Normal size":"Double size");
										ShowSpectrum();
										Refresh();
									};
			btnZoom.Enable(false);

	Add(acSS.HSizePosZ().VSizePos(310));
	
}

void SpectrumPanel::Paint(Draw &drw)
{
	//Size sz=GetSize();
	
	std::string s;
	int i, xr=SG.GetRect().left, y, yd, h=(nWinSize/2);
	double Hz, F0;

	F0=double(pVANA->VC.vio.Rate)/double(nWinSize);
	
	for (i=0;i<=h;i++)
	{
		Hz=(double(h-i)*F0);
		y=(i-SG.VS);
		if ((y<=256)&&((i==0)||(i==(3*h/4))||(i==(h/2))||(i==(h/4))||(i==h)))
		{
			if (i>0) y++;
			yd=(i>10)?(-5):0;
			drw.DrawText(5, y+yd, spf(Hz).c_str(), StdFont().Height(10), Gray());
			drw.DrawLine(xr-5, y, xr, y, 1, Black());
		}
	}
	y=(SG.mvpt.y);
	Hz=((h-(y+SG.VS))*F0);
	yd=(y>10)?(-5):0;
	drw.DrawText(5, y+yd, spf(Hz).c_str(),StdFont().Height(10), Red());
}

void SpectrumPanel::ShowSpectrum()
{
	auto reverse_vec=[&](VCD &vcd){ VCD v; for (auto d:vcd) v.insert(v.begin(), d); vcd=v; };
	Voice v;

	WaitCursor wait;

	vvcd.clear();
	pVANA->GetSound(v);
	pVANA->PFilters.DeNoise(v);
	if ((int)v.size()>nWinSize)
	{
		size_t i, j, n, m;
		double da, mag, amp, damax=0.0, damin=1000000.0;
		VCD vcd;

		n=(v.size()-nWinSize); //lose some at end
		for (i=0;i<=n;i+=nStep)
		{
			j=0;
			vcd.clear();
			for (j=0;(int)j<nWinSize;j++) vcd.push_back(ComplexDbl(v[i+j], 0));
			fft(vcd);
			m=(vcd.size()/2);
			vcd.erase(vcd.begin());
			vcd.erase(vcd.begin()+m, vcd.end());
			for (int y=0;y<(int)m;y++) //get min/max, calc 'tudes, ignore mirror
			{
				da=sqrt(vcd[y].real()*vcd[y].real() + vcd[y].imag()*vcd[y].imag());
				mag=da; amp=(da/m);
				if (da>damax) damax=da;
				if (da<damin) damin=da;
				vcd[y].real(amp); //*** NB: re-using as (r)amplitude ***********
				vcd[y].imag(mag); //*** and (i)magnitude ***********************
			}
			
			reverse_vec(vcd); //easier to plot later
			vvcd.push_back(vcd);
		}

		double dmedian=((damax+damin)/2), ddiff=(damax-damin+1);

//		do i still have this?
		/*mais::*/RunStats<double> RS;
		
		if (/*mais::*/is_zero(ddiff)) ddiff=1.0;
		for (auto &vp:vvcd) //adjust *tudes
		{
			for (auto &cn:vp)
			{
				da=(cn.imag()/ddiff);
				if (cn.imag()<dmedian) RS.Add(da); //to get average of all magnitudes<dmedian
				cn.imag(da);
				//da=(cn.real());
				//cn.real(da);
			}
		}

		//set too small magnitudes to 0 (remove clutter)
		for (auto &vp:vvcd) { for (auto &cn:vp) { if (cn.imag()<RS.Mean()) cn.imag(0.0); } }

	}
	btnZoom.Enable((vvcd.size()>0));
	SG.Enable((vvcd.size()>0));
	SG.ffpt.x=(-1);
	SG.ShowSpectrum();
}

bool SpectrumPanel::get_formant_frequencies(const VCD &V, VF &vf)
{

	///this is not right - fix...
	//...for each step .. get max if !=F0 then ==formant...

	double freq, mag, F0;//, da;
	int i, j, jj, k, n=V.size(), fn=0, fs=(-1);
	int step=4;//(n/32);
	/*mais::*/RunStats<double> RS;
	
	F0=double(pVANA->VC.vio.Rate)/double(nWinSize); //freq inc per bin
	
	for (auto fm:V) RS.Add(fm.imag());
	
	mag=V[n-1].imag();
	vf.push_back(Formant(F0, mag, 0, fs));
	
	i=(n-(step/2)-1);
	while (i>0)
	{
		mag=0;
		k=0;
		for (j=0;j<(step*2);j++)
		{
			jj=(i-j);
			if (jj>=0) if (mag<V[jj].imag()) { k=(jj); mag=V[k].imag(); }
			else j=(step*2);
		}
		if (mag>((RS.Mean()+RS.Median())/2))
		{
			fn++;
			freq=((n-k)*(F0));
			vf.push_back(Formant(freq, mag, fn, fs));
			i=k;
		}
		i-=(step/2);
	}
	return (vf.size()>0);
}

void SpectrumPanel::FillSpectralSection()
{
	acSS.Clear();
	acSS.Reset();
	acSS.AddColumn("Frequency").Sorting();
	acSS.AddColumn("Magnitude").Sorting();
	acSS.AddColumn("F#").Sorting();
	acSS.AddColumn("???");//Shape");
	
	if ((SG.ffpt.x>=0)&&(SG.ffpt.x<(int)vvcd.size()))
	{
		VF vf;
		std::string shape;
		if (get_formant_frequencies(vvcd[SG.ffpt.x], vf))
		{
			for (auto f:vf)
			{
				if (f.FShape>=0) shape=spf(f.FShape); else shape=" ";
				acSS.Add(f.freq, f.mag, f.FNum, shape.c_str());
			}
		}
	}
}


