
#include "vana.h"

SmallGraphCtrl::SmallGraphCtrl()
{
	pV=nullptr;
	bCompact=true;

	SetRect(0,0,100,138);
	Add(HS.HSizePosZ().BottomPos(0,10)); HS.SetLine(10); HS.Set(0,0,0); HS.WhenScroll << [&]{ Refresh(); };
}


void SmallGraphCtrl::Paint(Draw &drw)
{
	Size sz=GetSize();
	drw.DrawRect(sz,Black());
	drw.DrawLine(0,64,sz.cx,64,1,Color(64,64,0));
	
	if (pV)
	{
		int N=pV->size();
		if (bCompact&&(N>sz.cx))
		{
			int i=1, x1=0, y1=pV->at(0), x2, y2;
			double d=(double(sz.cx)/double(N));
			while (i<N)
			{
				y2=(pV->at(i)/2);
				x2=(int)(double(i)*d);
				drw.DrawLine(x1,128-y1,x2,128-y2,1,Cyan());
				x1=x2; y1=y2;
				i++;
			}
		}
		else
		{
			int n=((HS+N)<(HS+sz.cx))?(HS+N):(HS+sz.cx);
			int x1=0, y1=pV->at(HS), x2=HS, y2;
			while (x2<n)
			{
				y2=(pV->at(x2)/2);
				drw.DrawLine(x1,128-y1,x2-HS,128-y2,1,Cyan());
				x1=x2-HS; y1=y2;
				x2++;
			}
		}
	}
}

void SmallGraphCtrl::Plot(const Voice *pv, bool compact)
{
	Size sz=GetSize();
	pV=pv;
	bCompact=(sz.cx>100)?compact:false;
	if (bCompact) HS.Set(0,0,0);
	else { HS.SetTotal(pV->size()); HS.Set(0); }
	Refresh();
}

