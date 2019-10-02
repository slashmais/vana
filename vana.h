#ifndef _vana_h
#define _vana_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <vioctrl/vioctrl.h>
#include <utilfuncs/utilfuncs.h>

#include <vector>
#include <utility>
#include "fft.h"


#define LAYOUTFILE <vana/vana.lay>
#include <CtrlCore/lay.h>


//===============================================================================
struct VANA;
struct Panel : public Ctrl //for shared stuff ...
{
	typedef Panel CLASSNAME;
	VANA *pVANA;
	Panel(VANA *par=nullptr) : pVANA(par) {}
	virtual ~Panel(){};
};

//===============================================================================
struct SettingsPanel : public Panel
{
	typedef SettingsPanel CLASSNAME;

//	LabelBox lbSoundDef;
//	Label ll1, ll2, ll3;
//	Label lr1, lr2, lr3;

	LabelBox lbALSA;
	Label lblFormat, lblAccess, lblChannels, lblRate; //, lbl3, lbl4;
	DropList dlFormat, dlAccess;
	EditInt ebChannels, ebRate; //, ebTo, ebSelSize;
	Button btnApply, btnReset;

	SettingsPanel();
	virtual ~SettingsPanel() {}
	
	Callback WhenChange;
	
	void FillDLFormat();
	void FillDLAccess();

	//void SelectDlFormat(const std::string &s);
	//void SelectDLAccess(const std::string &s);
	
	void SetDefaults(); //reset to start-up defaults
	void SetALSA(snd_pcm_format_t format, snd_pcm_access_t access, unsigned int channels, unsigned int rate);
	
	int GetFormat();
	int GetAccess();
	int GetChannels();
	int GetRate();
	
};

//===============================================================================in smgctrl.cpp
struct SmallGraphCtrl : public Ctrl
{
	typedef SmallGraphCtrl CLASSNAME;
	
	HScrollBar HS;
	Voice const *pV;

	Point mvpt, ldpt;
	std::vector<Point> vld;
	bool bCompact;
	
	SmallGraphCtrl();
	virtual ~SmallGraphCtrl(){}

	void Layout() { HS.SetPage(GetSize().cx); }
	void Paint(Draw &drw);
	void Clear() { pV=nullptr; Refresh(); }
	void Plot(const Voice *pv, bool compact=true);
	
//	virtual void RightDown(Point p, dword keyflags);
//	virtual void LeftDown(Point p, dword keyflags);
//	virtual void LeftUp(Point p, dword keyflags);
//	virtual void MouseMove(Point p, dword keyflags);
//	virtual void MouseEnter(Point p, dword keyflags);
//	virtual void MouseLeave();
//	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	
};

//===============================================================================
struct SpectrumPanel : public Panel
{
	typedef SpectrumPanel CLASSNAME;
	
	enum { BASE_STEP_SIZE=32, BASE_WIN_SIZE=512, };
	
	struct SGramCtrl : public Ctrl
	{
		typedef SGramCtrl CLASSNAME;
		
		SpectrumPanel &SP;
		HScrollBar HS;
		VScrollBar VS;
		Image SGPic;
		Point mvpt, ldpt, ffpt;
		//std::vector<Point> vld;
		
		Callback WhenLU;
		
		SGramCtrl(SpectrumPanel &par);
		virtual ~SGramCtrl(){}
		virtual void Layout();

		void ShowMagnitudes(Draw &drw, int W, int H);
		void Paint(Draw &drw);
		void ShowSpectrum();
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseEnter(Point p, dword keyflags);
		virtual void MouseLeave();
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
	};

	struct Formant
	{
		double freq, mag;
		int vidx, FNum, FShape; //vidx==index into vvcd
		Formant(double f, double m, int N, int S) : freq(f), mag(m), FNum(N), FShape(S) {}
	};
	typedef std::vector<Formant> VF;

	SGramCtrl SG;
	Option optCols;
	Button btnSpectrum, btnZoom;
	LabelBox lb;
	Label lSA;
	EditDoubleNotNullSpin EdSA;
	ArrayCtrl acSS; //SpectralSection
	
	int nWinSize, nStep;
	VVCD vvcd;
	bool bShowFreq, bInvertColors, bZoom;
	double adjust_intensity, noise_level;
	
	SpectrumPanel(VANA *par);
	virtual ~SpectrumPanel() {}
	
	virtual void Paint(Draw &drw);
	void ShowSpectrum();
	bool get_formant_frequencies(const VCD &vcd, VF &vf);
	void FillSpectralSection(); //from callback
};

//===============================================================================
struct FilterPanel : public Panel
{
	typedef FilterPanel CLASSNAME;

	int delta;
	Label lbDelta;
	EditString ebDelta;
	Button btnDeNoise;
	
	FilterPanel(VANA *par);
	virtual ~FilterPanel() {}

	void DeNoise(Voice &v);
};

//===============================================================================
struct MixerPanel : public Panel
{
	typedef MixerPanel CLASSNAME;
	
	Label lT, lM, lB;
	SmallGraphCtrl TG, MG, BG;
	Button btnMix, btnUnMix, btnMakeCurrent;
	Voice vMix;
	
	MixerPanel(VANA *par);
	virtual ~MixerPanel() {}
	
	void SetSound(int tm, const Voice *pv, const std::string scap="<noname>");
	void Mix(bool bMix);

	//void Paint(Draw &drw);
	

};

//===============================================================================
struct PhoneticPanel : public Panel
{
	typedef PhoneticPanel CLASSNAME;
	
	Label l;
	
	PhoneticPanel();
	virtual ~PhoneticPanel() {}

};

//===============================================================================
struct ToysPanel : public Panel
{
	typedef ToysPanel CLASSNAME;
	
	Voice V;
	
	LabelBox lbSine;
	Label lblFreq, lblAmplitude, lblDuration;
	EditDoubleNotNull ebFreq, ebAmplitude, ebDuration;
	Button btnSineWave;
	Button btnTLStar, btnTLStarTreble, btnAGrace, btnStepAll, btnOwnC;
	Button btn4Chords;
	
	Callback3<double,double,double> WhenSineWave;
	
	ToysPanel(VANA *par);
	virtual ~ToysPanel() {}

	void OnSineWave();
	//void OnTLStar();
	//void OnAGrace();
	//void OnOwnC();

};

//===============================================================================
struct VoiceName : public TopWindow
{
	typedef VoiceName CLASSNAME;
	EditString eb;
	Button bk, bc;
	bool b;
	virtual ~VoiceName() {}
	void Set(const std::string &t, const std::string &s)
	{
		Title(t.c_str());
		SetRect(0, 0, 300, 80);
		//CenterScreen();
		CenterOwner();
		b=false;
		Add(eb.LeftPos(10,280).TopPos(10));
		Add(bk.SetLabel("OK").LeftPos(60, 60).TopPos(45, 20));
		Add(bc.SetLabel("Cancel").LeftPos(180, 60).TopPos(45, 20));
		eb.SetData(s.c_str());
		eb.SelectAll();
		bk.WhenPush << [&]{ std::string S=eb.GetData().ToString().ToStd(); /*mais::*/TRIM(S); if (!S.empty()) { b=true; Close(); } };
		bc.WhenPush << [&]{ Close(); };
	}
};

//===============================================================================
struct VANA : public TopWindow
{
	typedef VANA CLASSNAME;

	std::map<std::string, std::pair<Voice, bool> > MVoice;

	VIOCtrl VC;
	SettingsPanel PSettings;
	SpectrumPanel PSpectrum;
	FilterPanel PFilters;
	MixerPanel PMixer;
	PhoneticPanel PPhonetics;
	ToysPanel PToys;

	ArrayCtrl AR;
	TabCtrl TAB;
	
	Button btnSaveSession, btnLoadSession;
	
	Voice sound;

	VANA();
	virtual ~VANA() {}

	void SetAR();
	void ARMenu(Bar &bar);
	void ARSetMixer(int m, int g);
	void ARAppendToCurrent(int m);
	void SetupTabs();

	void ResetVANA();
	
	void OnSaveSession();
	void OnLoadSession();

	void SetCurrentVoice(const Voice &v);
	void OnClear() { Voice v; v.clear(); SetCurrentVoice(v); }
	
	void OnNewRecording(const Voice &v);
	void OnAddSound(const Voice &v);
	bool AddARSound(const Voice &v, const std::string &sn);
	void SetARName(const Voice &v, const std::string &sn);
	
	void GetSound(Voice &v);
	
	void OnARLDbl();
	
//	void OnSineWave(double f, double a, double d);

	
//	void ShowSampleStats();
//	void OnShowRaw();
//	void OnTest();
//	void OnTest2();
//	void ShowVoiceStats();
//	void TestLowPassFilter();
//	void TestLowPassFilter2();
//	void TestClearNoise();
//	void TestClearNoiseTrend();

	
};




#endif
