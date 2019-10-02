
#include "vana.h"
#include "piano.h"
//#include "maisutils.h"


ToysPanel::ToysPanel(VANA *par) : Panel(par)
{
	Add(lbSine.SetLabel("Set Sine Wave").LeftPos(5,600).TopPos(5,125));
	
	Add(lblFreq.SetLabel("Frequency:").LeftPos(10,80).TopPos(22));
	lblFreq.SetAlign(ALIGN_RIGHT);
	Add(ebFreq.MinMax(0.00125, 3999.9999).LeftPos(95,80).TopPos(20)); //need start + 4 points for one cycle = 8000/4 = 2000
	ebFreq = 1000.0; //0.00125;
	
	Add(lblAmplitude.SetLabel("Amplitude:").LeftPos(10,80).TopPos(47));
	lblAmplitude.SetAlign(ALIGN_RIGHT);
	Add(ebAmplitude.MinMax(1.0, 127.0).LeftPos(95,80).TopPos(45));
	ebAmplitude = 50.0;
	
	Add(lblDuration.SetLabel("Time(sec):").LeftPos(10,80).TopPos(72));
	lblDuration.SetAlign(ALIGN_RIGHT);
	Add(ebDuration.MinMax(0.00125, 86400.0).LeftPos(95,80).TopPos(70)); // ~24 hours
	ebDuration = 1.0; //0.00125;

	Add(btnSineWave.SetLabel("Apply").LeftPos(95,60).TopPos(100,20));
	btnSineWave.WhenPush = THISBACK(OnSineWave);

	Add(btnTLStar.SetLabel("Twinkle Little Star").LeftPos(200,150).TopPos(20));
	btnTLStar.WhenPush << [&]{ Voice v; TwinkleStar(v); pVANA->SetCurrentVoice(v); };
	Add(btnTLStarTreble.SetLabel(".. treble").LeftPos(360,60).TopPos(20));
	btnTLStarTreble.WhenPush << [&]{ Voice v; TwinkleStar_Treble(v); pVANA->SetCurrentVoice(v); };
	
	Add(btnAGrace.SetLabel("Amazing Grace").LeftPos(200,150).TopPos(40));
	btnAGrace.WhenPush << [&]{ Voice v; AmaGrace(v); pVANA->SetCurrentVoice(v); };
	
	Add(btnStepAll.SetLabel("High-to-Low-to-High").LeftPos(200,150).TopPos(60));
	btnStepAll.WhenPush << [&]{ Voice v; StepAllKeys(v); pVANA->SetCurrentVoice(v); };

	Add(btnOwnC.SetLabel("Own Composition!").LeftPos(200,150).TopPos(80));
	btnOwnC.WhenPush << [&]{ Voice v; OwnCompo(v); pVANA->SetCurrentVoice(v); };
	
	Add(btn4Chords.SetLabel("4 Chords").LeftPos(200,150).TopPos(100));
	btn4Chords.WhenPush << [&]{ Voice v; FourChords(v); pVANA->SetCurrentVoice(v); };
	
}

void ToysPanel::OnSineWave()
{
	double f=0.0, a=0.0, t=0.0;
	std::string s;
	s=ebFreq.GetData().ToString().ToStd(); /*mais::*/TRIM(s); if (!s.empty()) f=/*mais::*/stot<double>(s);
	s=ebAmplitude.GetData().ToString().ToStd(); /*mais::*/TRIM(s); if (!s.empty()) a=/*mais::*/stot<double>(s);
	s=ebDuration.GetData().ToString().ToStd(); /*mais::*/TRIM(s); if (!s.empty()) t=(/*mais::*/stot<double>(s));
	if ((f>0.0)&&(a>0.0)&&(t>0))
	{
		Voice s;
		WaitCursor wait;
		s.MakeSineWave(f, a, t, pVANA->VC.vio.Rate);
		pVANA->SetCurrentVoice(s);
	}
}


