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
const char AGENT='A';
const char SEPARATED='.';
const Position NULL_POS={-1,-1};
struct Cell
{
	bool check[5]={false,false,false,false,false};
};
void inp(int& N,vector<vector<Cell> >& a);
vector<Position> bfs(vector<vector<bool> >& matrix,const Position& source,const Position& destination);