#include <iostream>
#include <string>
#include <random>
#include <conio.h>

using namespace std;

const int SIZE = 20;
char board[SIZE][SIZE];
int p1_r, p1_c, p2_r, p2_c;
int score1 = 0, score2 = 0;
int remainingItems = 10;
bool p1_willBeObstacle = false;
bool p2_willBeObstacle = false;

random_device rd;
default_random_engine dre{ rd() };

void placeObstacle(int h, int w) {
	uniform_int_distribution<int> row_dist(0, SIZE - h);
	uniform_int_distribution<int> col_dist(0, SIZE - w);

	while (true) {
		int r = row_dist(dre);
		int c = col_dist(dre);

		// 겹침 확인
		bool canPlace = true;
		for (int i = r; i < r + h; ++i) {
			for (int j = c; j < c + w; ++j) {
				if (board[i][j] != 'O') canPlace = false;
			}
		}

		if (canPlace) {
			for (int i = r; i < r + h; ++i) {
				for (int j = c; j < c + w; ++j) board[i][j] = ' '; // ' '를 장애물로 설정
			}
			break;
		}
	}
}

void initGame() {
	score1 = 0; score2 = 0; remainingItems = 10;
	p1_willBeObstacle = false; p2_willBeObstacle = false;

	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j) board[i][j] = 'O';

	placeObstacle(3, 4);
	placeObstacle(5, 2);
	placeObstacle(4, 4);

	uniform_int_distribution<int> pos_dist(0, SIZE - 1);
	for (int i = 0; i < 10; ) {
		int r = pos_dist(dre);
		int c = pos_dist(dre);
		if (board[r][c] == 'O') {
			board[r][c] = '!';
			i++;
		}
	}

	p1_r = 0; p1_c = 0; p2_r = 19; p2_c = 19;
	board[p1_r][p1_c] = '#'; board[p2_r][p2_c] = '@';
}

void draw() {
	system("cls");
	cout << "P1 Score: " << score1 << " | P2 Score: " << score2 << " | Items: " << remainingItems << endl << endl;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) cout << board[i][j] << " ";
		cout << endl;
	}
}

void movePlayer(int& r, int& c, int dr, int dc, char symbol, int& score, bool& currentWillBeObstacle) {
	int nr = (r + dr + SIZE) % SIZE;
	int nc = (c + dc + SIZE) % SIZE;

	if (board[nr][nc] == ' ') return;


	if (currentWillBeObstacle) {
		board[r][c] = ' '; 
	}
	else {
		board[r][c] = 'O'; 
	}

	if (board[nr][nc] == '!') {
		score++;
		remainingItems--;
		currentWillBeObstacle = true; 
	}
	else {
		currentWillBeObstacle = false;
	}

	r = nr; c = nc;
	board[r][c] = symbol;
}

int main() {
	initGame();

	while (remainingItems > 0) {
		draw();
		char input = _getch();

		if (input == 'q') break;
		if (input == 'r') { initGame(); continue; }

		// 플레이어 1 이동 
		if (input == 'w') movePlayer(p1_r, p1_c, -1, 0, '#', score1, p1_willBeObstacle);
		else if (input == 's') movePlayer(p1_r, p1_c, 1, 0, '#', score1, p1_willBeObstacle);
		else if (input == 'a') movePlayer(p1_r, p1_c, 0, -1, '#', score1, p1_willBeObstacle);
		else if (input == 'd') movePlayer(p1_r, p1_c, 0, 1, '#', score1, p1_willBeObstacle);

		// 플레이어 2 이동
		if (input == 'i') movePlayer(p2_r, p2_c, -1, 0, '@', score2, p2_willBeObstacle);
		else if (input == 'k') movePlayer(p2_r, p2_c, 1, 0, '@', score2, p2_willBeObstacle);
		else if (input == 'j') movePlayer(p2_r, p2_c, 0, -1, '@', score2, p2_willBeObstacle);
		else if (input == 'l') movePlayer(p2_r, p2_c, 0, 1, '@', score2, p2_willBeObstacle);
	}

	// 종료 조건
	system("cls");
	cout << "=== GAME OVER ===" << endl;
	cout << "P1 Score: " << score1 << " | P2 Score: " << score2 << endl;
	if (score1 > score2) cout << "Winner: Player 1 (#)" << endl;
	else if (score2 > score1) cout << "Winner: Player 2 (@)" << endl;
	else cout << "It's a Tie!" << endl;

	system("pause");
	return 0;
}