#include "Wumpus.h"

//GLOBAL VARIABLES
vector<vector<bool>> limitArea;
queue<Position> safeArea;
int N;
vector<vector<Cell>> a;
Position current;
vector<Position> path;
vector<pair<Position, Cell>> kb;

void init();
void testInput();
void run();
void PL(vector<pair<Position, Cell>> newLaw);

int main()
{
	init();
	testInput();
}

void init() {
	vector<bool> tmp;
	for (int i = MIN_X - 1; i <= MAX_X + 1; i++) {
		for (int j = MIN_Y - 1; j <= MAX_Y + 1; j++)
			tmp.push_back(false);
		limitArea.push_back(tmp);
		tmp.clear();
	}

	limitArea[1][1] = limitArea[1][2] = limitArea[2][1] = true;
	safeArea.push(make_pair(1, 2));
	safeArea.push(make_pair(2, 1));

	current = make_pair(1, 1);
	path.push_back(current);
}

void testInput() {
	inp(N, a);
	int i, j;
	for (i = 1; i <= N; ++i)
	{
		for (j = 1; j <= N; ++j)
		{
			if (a[i][j].check[0]) cout << WUMPUS;
			if (a[i][j].check[1]) cout << PIT;
			if (a[i][j].check[2]) cout << BREEZE;
			if (a[i][j].check[3]) cout << STENCH;
			if (a[i][j].check[4]) cout << AGENT;
			cout << EMPTYROOM << SEPARATED;
		}
		cout << endl;
	}
}

void run() {
	Position destination;
	vector<Position> newPath;
	while (!safeArea.empty()) {

		destination = safeArea.front();
		safeArea.pop();
		newPath = bfs(limitArea, current, destination);
		path.insert(path.end(), newPath.begin(), newPath.end());
		current = destination;

		
	}
}

void entail(Cell x, Cell y) {
	
}

void PL(vector<pair<Position, Cell>> newLaw) {
	int kbSize, newLawSize;
	vector<pair<Position, Cell>> res;
	Cell tmp;
	if (kb.empty()) {
		kb.insert(kb.end(), newLaw.begin(), newLaw.end());
	}
	do {
		kbSize = kb.size();
		newLawSize = newLaw.size();
		for (int i = 0; i < kbSize; i++)
			for (int j = 0; j < newLawSize; j++)
				if (kb[i].first == newLaw[j].first) {
					
				}
	} while (res.size() != 0);
}