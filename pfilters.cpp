
#include "vana.h"
///#include "maisutils.h"
#include <utilfuncs/utilfuncs.h>
#include "vanautil.h"





FilterPanel::FilterPanel(VANA *par) : Panel(par)
{
	auto is_int = [](const std::string &s) ->bool { for (auto c:s) if ((c<'0')||(c>'9')) return false; return true; };
	delta=10;
	Add(lbDelta.SetLabel("Delta:").LeftPos(10,50).TopPos(10,20));
	Add(ebDelta.LeftPos(60,30).TopPos(10,20));
		ebDelta.SetData(delta);
		ebDelta.WhenAction << [&]
			{
				std::string s=ebDelta.GetData().ToString().ToStd();
				/*mais::*/TRIM(s);
				if (/*mais::*/is_int(s)) delta=/*mais::*/stot<int>(s);
				else { delta=10; ebDelta.SetData("10");}
			};
	Add(btnDeNoise.SetLabel("DeNoise").LeftPos(100,80).TopPos(10,20));
		btnDeNoise.WhenPush << [&]{ DeNoise(pVANA->sound); pVANA->SetCurrentVoice(pVANA->sound); };
}

void FilterPanel::DeNoise(Voice &v)
{
	// this will silence non-significant portions of a sound, it will NOT filter
	// noise from detected significant portions; significant sound portions will
	// be separated by silence that are exactly (N x 'step') number of units long
	// where N is an integer >= 1; silence less than 'step' units long are part
	// of a sound portion
	
	auto between=[](VoiceTypeUnit x, VoiceTypeUnit l, VoiceTypeUnit h){ return ((x>l)&&(x<h)); };
	
	WaitCursor wait;

	if (v.size()>0)
	{
		size_t i=0, j, n=v.size(), step=200;
		VoiceTypeUnit silence=128;
		/*mais::*/RunStats<VoiceTypeUnit> RS;

		while ((i+step)<n)
		{
			RS.Reset();
			
			for (j=0;j<step;j++) RS.Add(v[i+j]);
			if ((between(RS.Min(), RS.Mean()-delta, RS.Mean()+delta))
				&&(between(RS.Max(), RS.Mean()-delta, RS.Mean()+delta)))
			{
				for (j=0;j<step;j++) v[i+j]=silence;
			}
			i+=step;
		}
		///todo:...last part of v...
		//just silence it out for now..
		while (i<n) v[i++]=silence;
	}
	
	//alternative:
	// instead of silencing, build a Voice by concatenating detected noise
	// pass this Voice to fft and use detected freq's * amplitudes to 'unmerge'
	// from original
	
}
