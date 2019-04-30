#include "Wumpus.h"
int N; vector<vector<Cell> > a;
int main()
{
	inp(N,a);
	int i,j;
	for(i=1;i<=N;++i)
	{
		for(j=1;j<=N;++j)
		{
			if (a[i][j].check[0]) cout<<WUMPUS;
			if (a[i][j].check[1]) cout<<PIT;
			if (a[i][j].check[2]) cout<<BREEZE;
			if (a[i][j].check[3]) cout<<STENCH;
			if (a[i][j].check[4]) cout<<AGENT;
			cout<<EMPTYROOM<<SEPARATED;
		}
		cout<<endl;
	}
}