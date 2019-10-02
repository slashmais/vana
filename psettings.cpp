
#include "vana.h"
///#include "maisutils.h"
#include <utilfuncs/utilfuncs.h>


//SettingsPanel::SettingsPanel()
//{
//	Add(lbSoundDef.SetLabel("Raw Sound/Clip Description").SetFont(StdFont().Bold()).SetInk(Black()).LeftPos(5, 480).TopPos(5, 60));
//
//	Add(ll1.SetLabel("Type:").SetFont(StdFont().Bold()).SetInk(Black()).LeftPos(10, 50).TopPos(22)); ll1.SetAlign(ALIGN_RIGHT);
//		Add(lr1.SetLabel("Mono; unsigned 8-bit (converted (-128) to signed for graph)").LeftPos(65, 380).TopPos(22));
//	
//	Add(ll2.SetLabel("Rate:").SetFont(StdFont().Bold()).SetInk(Black()).LeftPos(10, 50).TopPos(42)); ll2.SetAlign(ALIGN_RIGHT);
//		Add(lr2.SetLabel("8000 units <-> 1 second (10 units <-> 1.25 ms)").LeftPos(65, 380).TopPos(42));
//
//}


/*
void testconfig(snd_pcm_stream_t stream, const char *device, const int *hwpars)
{
	unsigned min, max, param;
	int err, count, dir, result;

	printf("*** Exploring configuration space of device `%s' for %s ***\n", device,
	       stream==SND_PCM_STREAM_CAPTURE? "recording" : "playback");
	err= snd_pcm_open(&pcm, device, stream, SND_PCM_NONBLOCK);
	tc_errcheck(err, "opening sound device");
	err= snd_pcm_hw_params_any(pcm, pars);
	tc_errcheck(err, "initialising hardware parameters");
	for( count= 0; hwpars[count]!=HWP_END; count += 2 )

		switch(hwpars[count])
		{
		case HWP_RATE:param= hwpars[count+1];
			err= snd_pcm_hw_params_set_rate_near(pcm, pars, &param, &result);
			if( err<0 )
				fprintf(stderr, "Could not set sampling rate to %d Hz: %s.  "
				        "Continuing regardless.\n", hwpars[count+1], alsaerrstr(err));
			else
				printf("Set sampling rate %d Hz --> got %u Hz, %s requested.\n", hwpars[count+1], param, dirstr(dir));
			break;
		case HWP_NCH:err= snd_pcm_hw_params_set_channels(pcm, pars, hwpars[count+1]);
			if( err<0 )
				fprintf(stderr, "Could not set # of channels to %d: %s.  "
				        "Continuing regardless.\n", hwpars[count+1], alsaerrstr(err));
			else
				printf("Set number of channels to %d.\n", hwpars[count+1]);
			break;
		case HWP_FORMAT:err= snd_pcm_hw_params_set_format(pcm, pars, hwpars[count+1]);
			if( err<0 )
				fprintf(stderr, "Could not set sample format to %s: %s."
				        "  Continuing regardless.\n", alsafmtstr(hwpars[count+1]), alsaerrstr(err));
			else
				printf("Set sample format to %s.\n", alsafmtstr(hwpars[count+1]));
			break;
		default:
			break;
		}
	if( count>0 )
		printf("Parameter ranges remaining after these settings:\n");
	snd_pcm_hw_params_get_channels_min(pars, &min);
	snd_pcm_hw_params_get_channels_max(pars, &max);
	if( min==max )
		if( min==1 )
			printf("1 channel\n");
		else
			printf("%u channels\n", min);
	else
		printf("%u..%u channels\n", min, max);
	snd_pcm_hw_params_get_rate_min(pars, &min, NULL);
	snd_pcm_hw_params_get_rate_max(pars, &max, NULL);
	if( min==max )
		printf("Sampling rate %u Hz\nSample formats: ", min);
	else
		printf("Sampling rate %u..%u Hz\nSample formats: ", min, max);
	snd_pcm_hw_params_get_format_mask(pars, fmask);
	printfmtmask(fmask);
	printf("\n");
	result= snd_pcm_hw_params_get_sbits(pars);
	if( result >= 0 )    // only available if bit width of all formats is the same
		printf("Significant bits: %d\n", result);
	snd_pcm_close(pcm);
}
*/

SettingsPanel::SettingsPanel()
{

	Add(lbALSA.SetLabel("Configure ALSA").SetInk(Black()).SetFont(StdFont().Bold()).LeftPos(5, 550).TopPos(5, 150));

		Add(lblFormat.SetLabel("Format:").LeftPos(10, 80).TopPos(25));
			lblFormat.SetAlign(ALIGN_RIGHT);
		Add(dlFormat.LeftPos(95, 180).TopPos(23));
		
		Add(lblAccess.SetLabel("Access:").LeftPos(10, 80).TopPos(50));
			lblAccess.SetAlign(ALIGN_RIGHT);
		//Add(dlAccess.LeftPos(95, 180).TopPos(48));
	
		Add(lblChannels.SetLabel("Channels:").LeftPos(10, 80).TopPos(75));
			lblChannels.SetAlign(ALIGN_RIGHT);
		Add(ebChannels.Max(99).Min(1).MaxChars(2).LeftPos(95, 30).TopPos(73));
	
		Add(lblRate.SetLabel("Rate:").LeftPos(10, 80).TopPos(100));
			lblRate.SetAlign(ALIGN_RIGHT);
		Add(ebRate.Max(999999999).Min(1).MaxChars(9).LeftPos(95, 80).TopPos(98));
		
		Add(btnApply.SetLabel("Apply").LeftPos(95, 60).TopPos(128));
			btnApply.WhenPush << [&]{ if (WhenChange) WhenChange(); };
	
		Add(btnReset.SetLabel("Defaults").LeftPos(20, 60).TopPos(128));
			btnReset.WhenPush << [&]{ SetDefaults(); };

	FillDLFormat();
	FillDLAccess();

	SetDefaults();
}

void SettingsPanel::SetDefaults() { SetALSA(SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 8000); }

void SettingsPanel::SetALSA(snd_pcm_format_t format, snd_pcm_access_t access, unsigned int channels, unsigned int rate)
{
	int k;
	k=dlFormat.FindKey((int)format); if (k>=0) dlFormat.SetIndex(k);
	k=dlAccess.FindKey((int)access); if (k>=0) dlAccess.SetIndex(k);
	ebChannels.SetData((int)channels);
	ebRate.SetData((int)rate);
	if (WhenChange) WhenChange();
}

void SettingsPanel::FillDLFormat()
{
	int i=0;
	size_t p;
	std::string sf, sd;
	bool bLE=true; //isLE() - little endian
	bool bF=false;

	dlFormat.Clear();
	//dlFormat.Add(0, "xxxS8 - Signed 8 bit");
	//dlFormat.Add(1, "xxxU8 - Unsigned 8 bit");

	//for (i=2; i<=SND_PCM_FORMAT_LAST; i++)
	for (i=0; i<=SND_PCM_FORMAT_LAST; i++)
	{
		if (snd_pcm_format_name((snd_pcm_format_t)i)!=NULL)
		{
			sf=snd_pcm_format_name((snd_pcm_format_t)i);
			sd=sf.substr(1,2);
			if ((sf.substr(0,2)=="S8")||(sf.substr(0,2)=="U8")
				|| (((bLE&&(sf.substr(sf.size()-2)=="LE"))||(!bLE&&(sf.substr(sf.size()-2)=="BE")))
					&& ((sd=="16")||(sd=="32")||(sf.substr(0,5)=="FLOAT"))))
			{
				if (sf.substr(0,5)=="FLOAT") { sf=(bF)?"":"Floating point"; bF=true; }
				else
				{
					sf=snd_pcm_format_description((snd_pcm_format_t)i);
					if ((p=sf.find("Little"))!=std::string::npos) sf=sf.substr(0,p);
				}
				if (!sf.empty()) dlFormat.Add(i, sf.c_str());
			}
		}
	}
}

void SettingsPanel::FillDLAccess()
{
	int i=0;
	std::string sa;
	dlAccess.Clear();
	for (i=0; i<=SND_PCM_ACCESS_LAST; i++)
	{
		sa=snd_pcm_access_name((snd_pcm_access_t)i);
		if (sa[0]=='R') dlAccess.Add(i, sa.c_str());
	}
}

int SettingsPanel::GetFormat()
{
	int k;
	k=(int)dlFormat.GetIndex();
	k=(int)dlFormat.GetKey(k); //index for snd_pcm_format
	return k;
}
	
int SettingsPanel::GetAccess()
{
	int k;
	k=(int)dlAccess.GetIndex();
	k=(int)dlAccess.GetKey(k);
	return k;
}
	
int SettingsPanel::GetChannels()
{
	int k;
	std::string s;
	s=ebChannels.GetData().ToString().ToStd();
	k=stot<unsigned int>(s);
	return k;
}

int SettingsPanel::GetRate()
{
	int k;
	std::string s;
	s=ebRate.GetData().ToString().ToStd();
	k=stot<unsigned int>(s);
	return k;
}

