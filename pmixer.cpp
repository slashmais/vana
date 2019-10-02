
#include "vana.h"
//#include "maisutils.h"

//#include <maisutils.h>
//#include <VIO/vio.h>
//#include <string>
//#include <alsa/asoundlib.h>
//#include <cmath>



MixerPanel::MixerPanel(VANA *par) : Panel(par)
{
	vMix.clear();
	
	Add(lT.SetLabel("A: <empty>").HSizePosZ(5,1).TopPos(1,20));
	Add(TG.HSizePosZ(1,1).TopPos(21,138));

	Add(lM.SetLabel("B: <empty>").HSizePosZ(5,1).TopPos(160,20));
	Add(MG.HSizePosZ(1,1).TopPos(180,138));

	Add(btnMix.SetLabel("Merge (A+B)").LeftPos(5,100).TopPos(319,20)); btnMix.WhenPush=THISBACK1(Mix, true);
	Add(btnUnMix.SetLabel("Unmerge (A-B)").LeftPos(110,100).TopPos(319,20)); btnUnMix.WhenPush=THISBACK1(Mix, false);
	Add(BG.HSizePosZ(1,1).TopPos(339,138));
	Add(btnMakeCurrent.SetLabel("Make Current").LeftPos(5,100).TopPos(478,20));
	btnMakeCurrent << [&]{ pVANA->SetCurrentVoice(vMix); };
	
}

void MixerPanel::SetSound(int tm, const Voice *pv, const std::string scap)
{
	if (!pv) return;
	if (tm==1)
	{
		lT.SetLabel(/*mais::*/spf("A [%d]: %s", pv->size(), scap.c_str()).c_str());
		TG.Plot(pv);
	}
	else if (tm==2)
	{
		lM.SetLabel(/*mais::*/spf("B [%d]: %s", pv->size(), scap.c_str()).c_str());
		MG.Plot(pv);
	}
}

void MixerPanel::Mix(bool bMix)
{
	if (TG.pV && MG.pV)
	{
		vMix=*TG.pV;
		if (bMix) vMix.merge(*MG.pV); else vMix.unmerge(*MG.pV);
		BG.Plot(&vMix);
	}
}


