#include<iostream>
#include<string>
#include<algorithm>
#include<list>
#include <cstdlib>

using namespace std;

char sentence[40];
list<string> words;
list<int> spacecnts;
bool isrunning;

// 명령어 룰
// 1. a~z를 입력하면 대소문자를 바꾼다.
// 2. 1을 입력하면 공백을 줄인다.
// 3. 2를 입력하면 공백을 늘린다.
// 4. 3을 입력하면 알파벳 오름차순 정렬 + 개수까지 ex) a3b2c3d4e10...
// 5. 4를 입력하면 단어 길이 기준 정렬 + 개수 단어 사이의 공백은 한 칸
// 6. . 을 문장 마지막에 적는다.

void makewords(const char*);


//----------------
int main()
//----------------
{
	isrunning = true;
	cout << "문장을 입력하시오 : ";
	cin.getline(sentence, 40);

}

void makewords(char* sen)
{
	// sen을 순회하며 단어들을 저장한다.
	// sen을 순회하며 공백의 갯수들을 저장한다.
	// 이때 문장 안의 알파벳 개수가 40개를 초과하면 거기까지만 기록한다.
	// 어차피 40개를 초과하지 못하지만 문장의 마지막에 .이 없는 경우는 있을 수 있다.

	int cnt = 0;		// 문장의 처음에 공백이 있다면 그것은 무효로 간주한다.
	string word{};		// 
	int spacecnt = 0;
	char c{};			// 마지막이 글자였는지 빈칸이었는지 확인하기 위해.

	for (int i = 0; i < 39; ++i, ++sen) {
		if (*sen == '.')
			return;
		else if (*sen == ' ' && cnt > 0) {
				++spacecnt;
				++cnt;
			}
		else {
			word += *sen;
			
		}
			
	}
}
