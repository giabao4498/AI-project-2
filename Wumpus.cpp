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
					case GOLD: a[i].back().check[4]=true;
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
vector<Position> bfs(vector<vector<bool> >& matrix,const Position& source, Position destination)
{
	vector<vector<Position> > pre;
	pre.resize(matrix.size());
	int i,j;
	for(i=0;i<pre.size();++i)
	for(j=0;j<matrix[i].size();++j) pre[i].push_back(NULL_POS);
	queue<Position> q;
	q.push(source);
	do
	{
		for(i=0;i<LIM_DIRECTION;++i)
		if (matrix[q.front().first+DIRECTION[i].first][q.front().second+DIRECTION[i].second])
		{
			if (q.front()==destination) break;
			q.push({q.front().first+DIRECTION[i].first,q.front().second+DIRECTION[i].second});
			matrix[q.back().first][q.back().second]=false;
			pre[q.back().first][q.back().second]=q.front();
		}
		q.pop();
	}
	while (!q.empty());
	vector<Position> res;
	while (pre[destination.first][destination.second]!=NULL_POS)
	{
		res.push_back(destination);
		destination=pre[destination.first][destination.second];
	}
	res.pop_back();
	reverse(res.begin(),res.end());
	return res;
}
