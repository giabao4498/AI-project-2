#include "Wumpus.h"

//GLOBAL VARIABLES
vector<vector<bool>> limitArea;
queue<Position> safeArea;
int N;
vector<vector<Cell>> a;
Position current;
vector<Position> path;
vector<vector<pair<Position, Symbol>>> kb;

void init();
void testInput();
void run();
void PL(vector<vector<pair<Position, Symbol>>> newLaw);
bool entail(Symbol x, Symbol y, Symbol& res);
bool inRange(int x, int y);
const vector<vector<pair<Position, Symbol>>>& generateLaw(const Position& current);
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
	vector<pair<Position, Symbol>> component;
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

	component.push_back(make_pair(make_pair(1, 1), Symbol(0, 0)));
	kb.push_back(component);
	
	component.clear();
	component.push_back(make_pair(make_pair(2, 1), Symbol(0, 0)));
	kb.push_back(component);
	
	component.clear();
	component.push_back(make_pair(make_pair(1, 2), Symbol(0, 0)));
	kb.push_back(component);
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

const vector<vector<pair<Position, Symbol>>>& generateLaw(const Position& current) {
	vector<vector<pair<Position, Symbol>>> res;
	vector<pair<Position, Symbol>> component;
	if (!a[current.first][current.second].check[2] && !a[current.first][current.second].check[3]) {
		for (int i = 0; i < LIM_DIRECTION; i++)
			if (inRange(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second))
				if (!limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second]) {
					limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second] = true;
					safeArea.push(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second));
				}
		return;
	}
	if (a[current.first][current.second].check[2]) {
		for (int i = 0; i < LIM_DIRECTION; i++)
			if (inRange(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second))
				if (!limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second])
					component.push_back(make_pair(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second), Symbol(1, -1)));
		res.push_back(component);
	}
	if (a[current.first][current.second].check[3]) {
		component.clear();
		for (int i = 0; i < LIM_DIRECTION; i++)
			if (inRange(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second))
				if (!limitArea[current.first + DIRECTION[i].first][current.second + DIRECTION[i].second])
					component.push_back(make_pair(make_pair(current.first + DIRECTION[i].first, current.second + DIRECTION[i].second), Symbol(-1, 1)));
		res.push_back(component);
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
	vector<vector<pair<Position, Symbol>>> newLaw;
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

bool isSame(const vector<pair<Position, Symbol>>& x, const vector<pair<Position, Symbol>>& y) {
	if (x.size() != y.size())
		return false;
	int cnt = 0;
	for (int i = 0; i < x.size(); i++)
		for (int j = 0; j < y.size(); j++)
			if (x[i].first == y[j].first && x[i].second == y[j].second) {
				cnt++;
				break;
			}
	return (cnt == x.size());
}

void PL(vector<vector<pair<Position, Symbol>>> newLaw) {
	vector<vector<pair<Position, Symbol>>> res;
	bool isExisted;
	do {
		for (int i = 0; i < newLaw.size(); i++) {
			isExisted = false;
			for (int j = 0; j < kb.size(); j++)
				if (isSame(newLaw[i], kb[j]))
					isExisted = true;
			if (!isExisted)
				kb.push_back(newLaw[i]);
		}
		
	} while (res.size() != 0);
}