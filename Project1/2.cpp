#include <iostream>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <random>

using namespace std;

random_device rd;
mt19937 g(rd());
uniform_int_distribution<> dis(0, 9);

void init(int _board[10][10], int _cursor[2], int* _select, int* _sum);
void print(int _board[10][10], int _cursor[2], int* _select, int* _sum);
bool KeyInput(int _board[10][10], int _cursor[2], int* _select, int* _sum);

bool Blue[10];

int main()
{
    int board[10][10];
    int cursor[2];
    int select;
    int sum;
    init(board, cursor, &select, &sum);
    print(board, cursor, &select, &sum);

    while (KeyInput(board, cursor, &select, &sum))
    {
        print(board, cursor, &select, &sum);
    }
    return 0;
}

void init(int _board[10][10], int _cursor[2], int* _select, int* _sum)
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            _board[i][j] = j;
        }
        shuffle(begin(_board[i]), end(_board[i]), g);
    }
    _cursor[0] = dis(g);
    _cursor[1] = dis(g);
    *_select = -1;
    *_sum = -1;

    for (int i = 0; i < 10; ++i)
    {
        Blue[i] = false;
    }
}

void print(int _board[10][10], int _cursor[2], int* _select, int* _sum)
{
    system("cls");

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (_cursor[0] == i && _cursor[1] == j)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                int sum = _board[i][j];
                if (*_sum == 0)
                {
                    for (int i = 0; i < 10; ++i)
                    {
                        if (Blue[i])
                            sum += i * 10;
                    }
                    if (Blue[_board[i][j]])
                        sum -= _board[i][j];
                }
                cout << sum << " ";
            }
            else if (Blue[_board[i][j]])
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                cout << _board[i][j] << " ";
            }
            else
                cout << _board[i][j] << " ";

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
        cout << endl;
    }
}

bool KeyInput(int _board[10][10], int _cursor[2], int* _select, int* _sum)
{
    while (true)
    {
        // Q, R
        {
            if (GetAsyncKeyState('Q') & 0x0001)
                return false;
            if (GetAsyncKeyState('R') & 0x0001)
            {
                init(_board, _cursor, _select, _sum);
                break;
            }
        }

        // WASD
        {
            bool change = false;
            if (GetAsyncKeyState('A') & 0x0001)
            {
                _cursor[1] -= 1;
                if (_cursor[1] == -1)
                    _cursor[1] = 9;
                change = true;
            }
            else if (GetAsyncKeyState('D') & 0x0001)
            {
                _cursor[1] += 1;
                if (_cursor[1] == 10)
                    _cursor[1] = 0;
                change = true;
            }
            else if (GetAsyncKeyState('W') & 0x0001)
            {
                _cursor[0] -= 1;
                if (_cursor[0] == -1)
                    _cursor[0] = 9;
                change = true;
            }
            else if (GetAsyncKeyState('S') & 0x0001)
            {
                _cursor[0] += 1;
                if (_cursor[0] == 10)
                    _cursor[0] = 0;
                change = true;
            }
            if (change)
            {

                break;
            }
        }

        // ArrowKey
        {
            if (GetAsyncKeyState(VK_UP) & 0x0001)
            {
                int tmp[10];
                for (int i = 0; i < 10; ++i)
                    tmp[i] = _board[0][i];
                for (int i = 0; i < 9; ++i)
                    for (int j = 0; j < 10; ++j)
                        _board[i][j] = _board[i + 1][j];
                for (int i = 0; i < 10; ++i)
                    _board[9][i] = tmp[i];

                _cursor[0] -= 1;
                if (_cursor[0] == -1)
                    _cursor[0] = 9;
                break;
            }
            else if (GetAsyncKeyState(VK_DOWN) & 0x0001)
            {
                int tmp[10];
                for (int i = 0; i < 10; ++i)
                    tmp[i] = _board[9][i];
                for (int i = 9; i > 0; --i)
                    for (int j = 0; j < 10; ++j)
                        _board[i][j] = _board[i - 1][j];
                for (int i = 0; i < 10; ++i)
                    _board[0][i] = tmp[i];

                _cursor[0] += 1;
                if (_cursor[0] == 10)
                    _cursor[0] = 0;
                break;
            }
            else if (GetAsyncKeyState(VK_LEFT) & 0x0001)
            {
                int tmp[10];
                for (int i = 0; i < 10; ++i)
                    tmp[i] = _board[i][0];
                for (int i = 0; i < 9; ++i)
                    for (int j = 0; j < 10; ++j)
                        _board[j][i] = _board[j][i + 1];
                for (int i = 0; i < 10; ++i)
                    _board[i][9] = tmp[i];

                _cursor[1] -= 1;
                if (_cursor[1] == -1)
                    _cursor[1] = 9;
                break;
            }
            else if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
            {
                int tmp[10];
                for (int i = 0; i < 10; ++i)
                    tmp[i] = _board[i][9];
                for (int i = 9; i > 0; --i)
                    for (int j = 0; j < 10; ++j)
                        _board[j][i] = _board[j][i - 1];
                for (int i = 0; i < 10; ++i)
                    _board[i][0] = tmp[i];

                _cursor[1] += 1;
                if (_cursor[1] == 10)
                    _cursor[1] = 0;
                break;
            }
        }

        // 0 ~ 9
        {
            if (GetAsyncKeyState('0') & 0x0001)
            {
                Blue[0] = !Blue[0];
                break;
            }
            else if (GetAsyncKeyState('1') & 0x0001)
            {
                Blue[1] = !Blue[1];
                break;
            }
            else if (GetAsyncKeyState('2') & 0x0001)
            {
                Blue[2] = !Blue[2];
                break;
            }
            else if (GetAsyncKeyState('3') & 0x0001)
            {
                Blue[3] = !Blue[3];
                break;
            }
            else if (GetAsyncKeyState('4') & 0x0001)
            {
                Blue[4] = !Blue[4];
                break;
            }
            else if (GetAsyncKeyState('5') & 0x0001)
            {
                Blue[5] = !Blue[5];
                break;
            }
            else if (GetAsyncKeyState('6') & 0x0001)
            {
                Blue[6] = !Blue[6];
                break;
            }
            else if (GetAsyncKeyState('7') & 0x0001)
            {
                Blue[7] = !Blue[7];
                break;
            }
            else if (GetAsyncKeyState('8') & 0x0001)
            {
                Blue[8] = !Blue[8];
                break;
            }
            else if (GetAsyncKeyState('9') & 0x0001)
            {
                Blue[9] = !Blue[9];
                break;
            }
        }

        // enter
        {
            if (GetAsyncKeyState(VK_RETURN) & 0x0001)
            {
                if (*_sum == 0)
                {
                    *_sum = -1;
                    break;
                }
                else
                {
                    *_sum = 0;
                    break;
                }
            }
        }
    }
    return true;
}