#include<iostream>
#include<string>
#include<algorithm>
#include<list>
#include<vector>
#include<array>
#include<cstdlib>

using namespace std;

char sentence[40];
list<string> words;
vector<int> spacecnts;
array<int, 26> alphacnts{};
bool isrunning;

// 명령어 룰
//  a~z를 입력하면 대소문자를 바꾼다.
//  1을 입력하면 공백을 줄인다.
//  2를 입력하면 공백을 늘린다.
//  3을 입력하면 알파벳 오름차순 정렬 + 개수까지 ex) a3b2c3d4e10...
//  4를 입력하면 단어 길이 기준 정렬 + 개수 단어 사이의 공백은 한 칸
//  . 을 문장 마지막에 적는다.

void makewords(char*);


//----------------
int main()
//----------------
{
	isrunning = true;
	cout << "문장을 입력하시오 : ";
	cin.getline(sentence, 40);

	if (cin.fail()) {            // 40자를 초과해서 입력받아 에러가 발생했다면
		cin.clear();             // 1. 에러 플래그 초기화 (삐진 cin을 달래줌)
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 2. 버퍼에 남은 찌꺼기 싹 비우기
	}

	makewords(sentence);

	isrunning = true;
	string order{};

	bool isorder4 = false;

	while (isrunning) {
		system("cls");
		isorder4 = false;
		cout << "명령어를 입력하시오 : ";
		cin >> order;
		cout << endl << endl;

		cout << "변경 전 문장 : ";
		auto ib = words.begin();
		for (int i = 0; i < words.size(); ++i, ++ib) {
			cout << *ib;
			if (i < spacecnts.size()) {
				for (int j = 0; j < spacecnts[i]; ++j)
					cout << ' ';
			}
		}
		cout << "." << endl << endl;

		if (*order.data() >= 'a' && *order.data() <= 'z') {
			// 대소문자 바꾸기
			for (auto& word : words) {
				for (auto& c : word) {
					if (c == *order.data())
						c = char(toupper(c));
					else if(char(tolower(c)) == *order.data())
						c = char(tolower(c));
				}
			}
		}
		else if (*order.data() == '1') {
			// 공백 줄이기
			for (auto& i : spacecnts)
				if (i > 0)
					--i;
		}
		else if (*order.data() == '2') {
			// 1. 먼저 공백을 늘립니다.
			for (auto& i : spacecnts)
				if (i < 5)
					++i;

			// 2. 전체 길이를 체크하며 40자가 넘으면 뒤에서부터 한 글자씩 삭제합니다.
			while (true) {
				int totalLen = 0;
				auto it = words.begin();
				// 현재 문장의 총 길이 계산 (단어 길이 + 공백 길이)
				for (int i = 0; i < words.size(); ++i, ++it) {
					totalLen += (int)it->size();
					if (i < spacecnts.size()) {
						totalLen += spacecnts[i];
					}
				}

				// 40자 이하이거나 더 이상 지울 단어가 없으면 종료
				if (totalLen <= 40 || words.empty()) break;

				// 맨 뒤의 단어에서 한 글자 제거
				if (!words.back().empty()) {
					words.back().pop_back();
				}

				// 만약 단어가 한 글자도 남지 않게 되었다면 단어와 그 앞의 공백 데이터를 제거
				if (words.back().empty()) {
					words.pop_back();
					if (!spacecnts.empty()) {
						spacecnts.pop_back();
					}
				}
			}
		}
		else if (*order.data() == '3') {
			//오름차순 정렬
			cout << "오름차순 정렬 : ";
			for (int i = 0; i < 26; ++i) {
				if (alphacnts[i] != 0)
					cout << char(i + 'a') << alphacnts[i];
			}
			cout << endl << endl;
		}
		else if (*order.data() == '4') {
			//단어길이별 정렬
			isorder4 = true;
			list<string> temp{ words.begin(),words.end() };
			words.sort([](const string& a, const string& b) {
				return a.size() < b.size();
				}
			);
			cout << "변경 후 문장 : ";
			ib = words.begin();
			for (int i = 0; i < words.size(); ++i, ++ib) {
				cout << *ib;
				if (i != words.size() - 1)
					cout << ' ';
			}
			cout << "." << endl << endl << endl << endl << endl;
		}
		else if (*order.data() == '0') {
			isrunning = false;
		}
		else {
			cout << "명령어를 다시 입력하세요";
			system("pause");
		}
		if(!isorder4)
		{
			cout << "변경 후 문장 : ";
			ib = words.begin();
			for (int i = 0; i < words.size(); ++i, ++ib) {
				cout << *ib;
				if (i < spacecnts.size()) {
					for (int j = 0; j < spacecnts[i]; ++j)
						cout << ' ';
				}
			}
			cout << "." << endl << endl << endl << endl << endl;
		}
		system("pause");
	}

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
						// 글자였다가 빈칸이면 지금까지 기록된 word를 words에 추가
						// 빈칸이었다가 글자면 spacecnt를 spacecnts에 저장

	for (int i = 0; i < 39; ++i, ++sen) {
		if (*sen == '.' || *sen == '\0')
			break;
		else if (*sen == ' ' && cnt > 0) {
			if (c != ' ') {
				words.push_back(word);
				word = "";
			}
			++spacecnt;
			c = ' ';
		}
		else if(*sen != ' ') {
			if (c == ' ') {
				spacecnts.push_back(spacecnt);
				spacecnt = 0;
			}
			word += *sen;
			c = *sen;
			++cnt;
		}
	}

	if (!word.empty()) {
		words.push_back(word);
	}

	
	for (const auto& word : words) {
		for (const auto& c : word) {
			++alphacnts[tolower(c) - 'a'];
		}
	}
}
