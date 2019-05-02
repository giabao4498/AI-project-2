#pragma once
#include<fstream>
#include<vector>
#include<string>
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;
typedef pair<int,int> Position;
const string inpFile="map.txt";
const string outFile="result.txt";
const char EMPTYROOM='-';
const char WUMPUS='W';
const char PIT='P';
const char BREEZE='B';
const char STENCH='S';
const char GOLD='G';
const char SEPARATED='.';
const Position NULL_POS={-1,-1};
const int MIN_X = 1;
const int MAX_X = 10;
const int MIN_Y = 1;
const int MAX_Y = 10;
const short LIM_DIRECTION = 4;
const int MAX_STEP = 150;
const pair<short, short> DIRECTION[LIM_DIRECTION] = { {-1,0},{0,1},{1,0},{0,-1} };
struct Cell
{
	bool check[5]={false,false,false,false,false};
};
class Symbol {
public:
	int P = -1, W = -1;
	Symbol() {}
	Symbol(int _P, int _W) : P(_P), W(_W) {}
	bool operator==(const Symbol& anotherSymbol) const {
		return (P == anotherSymbol.P && W == anotherSymbol.W);
	}
};
void inp(int& N,vector<vector<Cell> >& a);
vector<Position> bfs(vector<vector<bool> > matrix,const Position& source, Position destination);