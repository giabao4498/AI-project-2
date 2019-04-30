#include "Wumpus.h"
void inp(int& N,vector<vector<Cell> >& a)
{
	ifstream f(inpFile);
	f>>N;
	int i,j; string s;
	a.resize(1);
	a[0].resize(N+1);
	for(i=1;i<=N;++i)
	{
		a.resize(i+1);
		a[i].resize(1);
		f>>s;
		for(j=0;j<s.size();++j)
		{
			a[i].resize(a[i].size()+1);
			do
			{
				switch (s[j])
				{
					case WUMPUS: a[i].back().check[0]=true; break;
					case PIT: a[i].back().check[1]=true; break;
					case BREEZE: a[i].back().check[2]=true; break;
					case STENCH: a[i].back().check[3]=true; break;
					case AGENT: a[i].back().check[4]=true;
				}
				++j;
			}
			while (j<s.size() && s[j]!=SEPARATED);
		}
		a[i].resize(N+2);
	}
	f.close();
	a.resize(N+2);
	a.back()=a[0];
}