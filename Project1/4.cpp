#include <iostream>
#include <string>
#include <random>
#include <cstdlib>


using namespace std;

random_device rd;
default_random_engine dre{ rd() };
uniform_int_distribution<int> uid{ 0,400 };

int main()
{
	// 20x20 크기의 판을 만든다.
	// 3x4 장애물 놓을 위치를 고른다.
	// 5x2 장애물 놓을 위치를 고른다.
	// 4x4 장애물 놓을 위치를 고른다.
	// 겹치면 다시 고른다.
	// 아이템 고른다.
	// 겹치면 다시 고른다.
	// 이동한다.
	char board[20][20]{};
	for (int i = 0; i < 20; ++i) { //보드판 초기화
		for (int j = 0; j < 20; ++j) {
			board[i][j] = 'O';
		}
	}

	bool check[3]{ false };
	bool check2{ false };
	while (true) {
		if (check[0] == false) {			// 장애물 1번 생성
			int position = uid(dre);
			int 몫 = position % 16;
			int 나머지 = position % 17;
			for (int i = 몫; i < 몫 + 4; ++i) {
				for (int j = 나머지; j < 나머지 + 3; ++j) {
					board[i][j] = ' ';
				}
			}
			check[0] = true;
		}

		else if (check[1] == false) {
			while (true) {
				int position = uid(dre);
				int 몫 = position % 18;
				int 나머지 = position % 15;
				bool 통과 = true;
				for (int i = 몫; i < 몫 + 2; ++i) {
					for (int j = 나머지; j < 나머지 + 5; ++j) {
						if (board[i][j] == ' ')
							통과 = false;
					}
				}
				if (통과 == true) {
					for (int i = 몫; i < 몫 + 2; ++i) {
						for (int j = 나머지; j < 나머지 + 5; ++j) {
							board[i][j] = ' ';
						}
					}
					break;
				}
			}

			check[1] = true;
		}

		else if (check[2] == false) {
			while (true) {
				int position = uid(dre);
				int 몫 = position % 16;
				int 나머지 = position % 16;
				bool 통과 = true;
				for (int i = 몫; i < 몫 + 4; ++i) {
					for (int j = 나머지; j < 나머지 + 4; ++j) {
						if (board[i][j] == ' ')
							통과 = false;
					}
				}
				if (통과 == true) {
					for (int i = 몫; i < 몫 + 4; ++i) {
						for (int j = 나머지; j < 나머지 + 4; ++j) {
							board[i][j] = ' ';
						}
					}
					break;
				}
			}

			check[2] = true;
			break;
		}
	}


	int i = 0;
	while (i < 10) {
		int randomi = uid(dre);
		if (board[randomi / 20 % 20][randomi % 20] != ' ') {
			++i;
			board[randomi / 20 % 20][randomi % 20] = '*';
		}
	}


	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}

	system("pause");
}