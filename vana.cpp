
#include "vana.h"
#include <utilfuncs/utilfuncs.h>
#include <VIO/vio.h>
#include <string>
#include <alsa/asoundlib.h>
#include <cmath>
#include "vanautil.h"


//===============================================================================debugging

#include <mutex>
std::mutex mtx_dbg;
int ddcount=0;
String Sdebug;
void dodebug(const std::string s)
{
	//if (!pVANA) return;
	if (s.empty()) { Sdebug.Clear(); return; }
	mtx_dbg.lock();
	String S;//=pVANA->ded.GetData().ToString();
	ddcount++;
	S << "\n" << ddcount << ": " << s.c_str();
	Sdebug+=S;
	mtx_dbg.unlock();
}


void telluser(const std::string s)
{
	PromptOK(DeQtf(s.c_str()));
}


//===============================================================================VANA

VANA::VANA() : PSpectrum(this), PFilters(this), PMixer(this), PToys(this)
{
	//Icon(......)
	Title("Voice Analysis");
	SetRect(0,0, 1000, 850);
	CenterScreen();
	Sizeable();

	//pVANA=this;

	MVoice.clear();
	sound.clear();
	
	int h=VC.GetSize().cy;//, t;
	
	Add(VC.HSizePosZ(1,1).TopPos(1, h));
	VC.WhenRecorded = THISBACK(OnNewRecording);
	VC.WhenVoice = THISBACK(OnAddSound);
	VC.WhenNew = THISBACK(OnClear);

	Add(AR.LeftPos(1, 270).VSizePosZ(h-35, 40));
	AR.WhenLeftDouble = THISBACK(OnARLDbl);
	AR.WhenBar = THISBACK(ARMenu);

	Add(btnSaveSession.SetLabel("Save Session..").LeftPos(10, 100).BottomPosZ(10, 20));
	btnSaveSession.WhenPush = THISBACK(OnSaveSession);

	Add(btnLoadSession.SetLabel("Load Session..").LeftPos(120, 100).BottomPosZ(10, 20));
	btnLoadSession.WhenPush = THISBACK(OnLoadSession);

	Add(TAB.HSizePosZ(210, 2).VSizePosZ(h-35, 2));
	SetupTabs();

	SetAR();
	
	ResetVANA();
	
}

//void VANA::Layout()
//{
//	Size sz=GetSize();
//	graph.HS.SetPage(sz.cx);
//}

void VANA::ResetVANA()
{
	
	//..

	VC.ClearVIO();

}

void VANA::OnSaveSession()
{
	telluser("...todo... use SQLite for sessions...");
}

void VANA::OnLoadSession()
{
	telluser("...todo... use SQLite for sessions...");
}

void VANA::SetCurrentVoice(const Voice &v) { sound=v; VC.SetVoice(sound); }
void VANA::OnNewRecording(const Voice &v) { if (AddARSound(v, "new recording")) SetCurrentVoice(v); }
void VANA::OnAddSound(const Voice &v) { AddARSound(v, "sound clip"); }

bool VANA::AddARSound(const Voice &v, const std::string &sn)
{
	VoiceName VN;
	VN.Set("Sound Name", sn);
	VN.Run(true);
	if (VN.b)
	{
		std::string st;
		st=VN.eb.GetData().ToString().ToStd();
		SetARName(v, st);
		SetAR();
		return true;
	}
	return false;
}

void VANA::SetARName(const Voice &v, const std::string &sn)
{
	int i=1;
	std::string st=sn;
	while (MVoice.find(st)!=MVoice.end()) { st=spf(sn.c_str(), "_", i); i++; }
	MVoice[st]=std::pair<Voice,bool>(Voice(v), false);
}

void VANA::GetSound(Voice &v)
{
	VC.ExtractClip(v);
}

void VANA::SetAR()
{
	AR.Reset();
	AR.Clear();
	AR.AddColumn("Voice Clip", 200);
	AR.AddColumn("Sel", 30).Ctrls<Option>();
	
	for (auto p:MVoice) { AR.Add(p.first.c_str(), p.second.second); }

}

void VANA::ARMenu(Bar &bar)
{
	int m=AR.GetCursor();
	if (m>=0)
	{
		bar.Add("Set to Mixer Top",THISBACK2(ARSetMixer, m, 1));
		bar.Add("Set to Mixer Mid",THISBACK2(ARSetMixer, m, 2));
		bar.Separator();
		bar.Add("Append to Current",THISBACK1(ARAppendToCurrent,m));
		bar.Separator();
		bar.Add("Delete",[&,m]
			{
				std::string s=AR.Get(m,0).ToString().ToStd();
				MVoice.erase(s);
				AR.Remove(m);
			});
	}
}

void VANA::ARSetMixer(int m, int g)
{
	if (m>=0)
	{
		std::string s=AR.Get(m,0).ToString().ToStd();
		PMixer.SetSound(g, &MVoice[s].first, s);
	}
}

void VANA::ARAppendToCurrent(int m)
{
	Voice v=sound;
	v.blendappend(MVoice[AR.Get(m,0).ToString().ToStd()].first);
	SetCurrentVoice(v);
}

void VANA::SetupTabs()
{
	TAB.Reset();
	
	TAB.Add(PSettings.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Settings");
	TAB.Add(PSpectrum.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Spectrum");
	TAB.Add(PFilters.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Filters");
	TAB.Add(PMixer.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Mixer");
	TAB.Add(PPhonetics.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Phonetics");
	TAB.Add(PToys.HSizePos().VSizePos().AddFrame(ThinInsetFrame()), "Toys");

}

void VANA::OnARLDbl()
{
	int C=AR.GetCursor();
	if (C>=0)
	{
		sound=MVoice[AR.Get(C,0).ToString().ToStd()].first;
		SetCurrentVoice(sound);
	}
}






//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

//void VANA::OnSineWave(double f, double a, double d)
//{
//	Voice s;
//	WaitCursor wait;
//	s.MakeSineWave(f, a, d, VC.vio.Rate);
//	SetCurrentVoice(s);
//}


//-------------------------------------------------------------------------------






///=============================================================================================================




//void VANA::ShowSampleStats()
//{
///*
//	std::string s;
//	int l,r;
//	
//	spf(s, "%llu", RData.size()); stats.ebCSSize.SetData(s.c_str());
//	if (graph.GetSelection(l,r))
//	{
//		spf(s, "%d", l); stats.ebFrom.SetData(s.c_str());
//		spf(s, "%d", r); stats.ebTo.SetData(s.c_str());
//		spf(s, "%d", (r-l+1)); stats.ebSelSize.SetData(s.c_str());
//	}
//	else
//	{
//		stats.ebFrom.SetData("0");
//		stats.ebTo.SetData(s.c_str());
//		stats.ebSelSize.SetData(s.c_str());
//	}
//	
//	btnShowRaw.Enable(!RData.empty());
//*/
//
//}
//
//
//void VANA::OnTest()
//{
//	//TestClearNoise();
//	//TestClearNoiseTrend();
//	//TestLowPassFilter2();
//
//
//	Voice s, sh, sl, shl;
//	size_t i=0, m=2500;
//	int h, l;
//
//	MakeSineWave(sl, 100.0, 50.0, m);
//	MakeSineWave(sh, 250.0, 30.0, m);
//	for (i=0;i<m;i++) { h=(sh[i]-128); l=(sl[i]-128); shl+=(uint8_t)(h+l+128); }
//	s=sh; s+=sl; s+=shl;
//	s+=s; s+=s; s+=s; s+=s; s+=s;
//	SetVoice(s);
//
//}
///*
//	RawVoice s, sh, sl, shl;
//	int i=0, h=0, l=0, ah=70, al=20, m=100;
//	
//	s.clear(); sh.clear(); sl.clear(); shl.clear();
//	
//	for (i=0; i<m; i++)
//	{
//		for (h=(-ah); h<ah; h+=2) sh+=(int8_t)(h+128);
//		for (h=ah; h>(-ah); h-=2) sh+=(int8_t)(h+128);
//		for (l=(-al); l<al; l++) sl+=(int8_t)(l+128);
//		for (l=al; l>(-al); l--) sl+=(int8_t)(l+128);
//	}
//	
//	h=sh.size(); l=sl.size();
//	m=(h<l)?h:l;
//	for (i=0;i<m;i++) { h=(sh[i]-128); l=(sl[i]-128); shl+=(uint8_t)(h+l+128); }
//	
//	s=sh; s+=sl; s+=shl;
//	graph.Plot(s);
//	OnRPStop();
//}
//*/
//
//void VANA::OnTest2()
//{
//}
//
//
//void VANA::ShowVoiceStats()
//{
///*
//	if (graph.GetSelection(VData))
//	{
//		VoiceStats SS;
//		std::string s;
//		
//		GetVoiceStats(VData, SS);
//		spf(s, "Interval = %llu\nMaxima = %llu\nMinima = %llu\naverage = %.2f\nmean = %.2f\nfrequency = %.2f\namplitude high = %.2f\namplitude low = %.2f\n",
//			SS.nInterval, SS.nMaxima, SS.nMinima, SS.daverage, SS.dmean, SS.dfreq, SS.damphigh, SS.damplow);
//		eb.SetData(s.c_str());
//	}
//*/
//}
//
//void VANA::TestLowPassFilter()
//{
////	if (graph.GetSelection(VData)) { LowPassFilter(VData); graph.Plot(VData); }
//}
//
//void VANA::TestLowPassFilter2()
//{
////	if (graph.GetSelection(VData)) { LowPassFilter2(VData); graph.Plot(VData); }
//}
//
//void VANA::TestClearNoise()
//{
///*
//	if (graph.GetSelection(VData))
//	{
//		ClearNoise(VData);
//		graph.Plot(VData);
//	}
//*/
//}
//
//void VANA::TestClearNoiseTrend()
//{
///*
//	struct GT : public TopWindow
//	{
//		typedef GT CLASSNAME;
//		GT() { SetRect(0,0,100,100); }
//		int Get()
//		{
//			Button bp,bf,bl;
//			int pfl;
//			Add(bp.SetLabel("Past").LeftPos(10,60).TopPos(10,20));
//			Add(bf.SetLabel("Future").LeftPos(10,60).TopPos(35,20));
//			Add(bl.SetLabel("Local").LeftPos(10,60).TopPos(60,20));
//			bp.WhenPush << [&](){ pfl=PAST_TREND; Close(); };
//			bf.WhenPush << [&](){ pfl=FUTURE_TREND; Close(); };
//			bl.WhenPush << [&](){ pfl=LOCAL_TREND; Close(); };
//			SetRect(0,0,100,100);
//			CenterOwner();
//			Execute();
//			return pfl;
//		}
//	};
//	
//	if (graph.GetSelection(VData))
//	{
//		int pf=GT().Get();
//		ClearNoiseTrend(VData, pf);
//		graph.Plot(VData);
//	}
//*/
//}



