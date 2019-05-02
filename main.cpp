#include "Wumpus.h"

//GLOBAL VARIABLES
vector<vector<bool>> limitArea;
queue<Position> safeArea;
int N;
vector<vector<Cell>> a;
Position current;
vector<Position> path;
vector<pair<Position, Symbol>> kb;

void init();
void testInput();
void run();
void PL(vector<pair<Position, Symbol>> newLaw);
bool entail(Symbol x, Symbol y, Symbol& res);
bool inRange(int x, int y);
vector<pair<Position, Symbol>> generateLaw(const Position& current);
void calculateScore(Position current);
int score = 0;
int step = 0;

int main()
{
	inp(N, a);
	init();
	//testInput();
	run();
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

	kb.push_back(make_pair(make_pair(1, 1), Symbol(0, 0)));
	kb.push_back(make_pair(make_pair(2, 1), Symbol(0, 0)));
	kb.push_back(make_pair(make_pair(1, 2), Symbol(0, 0)));
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
			if (a[i][j].check[4]) cout << GOLD;
			cout << EMPTYROOM << SEPARATED;
		}
		cout << endl;
	}
}

vector<pair<Position, Symbol>> generateLaw(const Position& current) {
	vector<pair<Position, Symbol>> res;
	if (a[current.first][current.second].check[2] || a[current.first][current.second].check[3]) {
		for (int i = 0; i < LIM_DIRECTION; i++)
			if (inRange(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second))
				if (!limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second]) {
					if (a[current.first][current.second].check[2] && a[current.first][current.second].check[3])
						res.push_back(make_pair(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second), Symbol(1, 1)));
					else if (a[current.first][current.second].check[2])
						res.push_back(make_pair(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second), Symbol(1, -1)));
					else if (a[current.first][current.second].check[3])
						res.push_back(make_pair(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second), Symbol(-1, 1)));
				}
	}
	else {
		for (int i = 0; i < LIM_DIRECTION; i++)
			if (inRange(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second))
				if (!limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second]) {
					limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second] = true;
					safeArea.push(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second));
				}
	}
	return res;
}

void calculateScore(Position current) {
	if (a[current.first][current.second].check[4])
		score += 100;
}

bool inRange(int x, int y) {
	return (x >= MIN_X && x <= MAX_Y && y >= MIN_X && y <= MAX_Y);
}

void run() {
	Position destination;
	vector<Position> newPath, home;
	vector<pair<Position, Symbol>> newLaw;
	while (!safeArea.empty()) {

		destination = safeArea.front();
		safeArea.pop();
		newPath = bfs(limitArea, current, destination);
		home = bfs(limitArea, destination, make_pair(1, 1));
		if (step + newPath.size() + home.size() > MAX_STEP) {
			if (current != make_pair(1, 1))
				path.insert(path.end(), home.begin(), home.end());
			break;
		}
		path.insert(path.end(), newPath.begin(), newPath.end());
		current = destination;
		calculateScore(current);
		newLaw = generateLaw(current);
		PL(newLaw);
	}
}

bool entail(Symbol x, Symbol y, Symbol& res) {
	bool check = false;
	res = x;
	if ((x.P = 1 && y.P == 0) || (x.P = 0 && y.P == 1)) {
		check = true;
		res.P = -1;
	}
	if ((x.W = 1 && y.W == 0) || (x.W = 0 && y.W == 1)) {
		check = true;
		res.W = -1;
	}
	return check;
}

bool isSafe(Symbol x) {
	if ((x.P == -1 && x.W == 0) || (x.P == 0 && x.W == -1))
		return true;
	return false;
}

void PL(vector<pair<Position, Symbol>> newLaw) {
	int kbSize;
	vector<pair<Position, Symbol>> res;
	Symbol tmp;
	bool isExisted;
	do {
		kbSize = kb.size();
		res.clear();
		for (int i = 0; i < newLaw.size(); i++) {
			isExisted = false;
			for (int j = 0; j < kb.size(); j++)
				if (newLaw[i].first == kb[j].first && newLaw[i].second == kb[j].second) {
					isExisted = true;
					break;
				}
			if (!isExisted)
				kb.push_back(newLaw[i]);
		}
		for (int i = 0; i < kbSize; i++)
			for (int j = 0; j < newLaw.size(); j++)
				if (kb[i].first == newLaw[j].first) {
					if (entail(kb[i].second, newLaw[j].second, tmp)) {
						isExisted = false;
						if (!limitArea[kb[i].first.first][kb[i].first.first] && isSafe(tmp)) {
							safeArea.push(kb[i].first);
							limitArea[kb[i].first.first][kb[i].first.first] = true;
						}
						for (int k = 0; k < kb.size(); k++)
							if (kb[k].first == kb[i].first && kb[k].second == tmp) {
								isExisted = true;
								break;
							}
						if (!isExisted) {
							res.push_back(make_pair(kb[i].first, tmp));
							kb.push_back(make_pair(kb[i].first, tmp));
						}
					}
				}
		newLaw.clear();
		newLaw = res;
	} while (res.size() != 0);
}