#pragma once
#include<fstream>
#include<vector>
#include<string>
#include<iostream>
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
struct Cell
{
	bool check[5]={false,false,false,false,false};
};
void inp(int& N,vector<vector<Cell> >& a);