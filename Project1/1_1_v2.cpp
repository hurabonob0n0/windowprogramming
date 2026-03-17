#include<iostream>
#include<string>
#include<algorithm>
#include<list>
#include<vector>
#include<array>
#include<cstdlib>
#include<limits> // numeric_limits 사용을 위해 추가

using namespace std;

char sentence[40];
list<string> words;
list<string> temp;      // 원본 순서를 저장할 리스트
bool isChanged = false; // 정렬 모드인지 확인하는 플래그
vector<int> spacecnts;
array<int, 58> alphacnts{};
bool isrunning;

void makewords(char*);

int main()
{
	isrunning = true;
	cout << "문장을 입력하시오 : ";
	cin.getline(sentence, 40);

	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	makewords(sentence);

	string order{};
	bool isorder4 = false;

	while (isrunning) {
		system("cls");
		isorder4 = false;
		cout << "명령어를 입력하시오 : ";
		cin >> order;
		cout << endl << endl;

		char cmd = order[0];

		// --- 변경 전 문장 출력부 ---
		// 정렬 상태라면 백업본(temp)을, 아니면 현재 words를 출력
		cout << "변경 전 문장 : ";
		auto& printTarget = isChanged ? temp : words;
		auto ib_prev = printTarget.begin();
		for (int i = 0; i < printTarget.size(); ++i, ++ib_prev) {
			cout << *ib_prev;
			if (i < spacecnts.size()) {
				for (int j = 0; j < spacecnts[i]; ++j) cout << ' ';
			}
		}
		cout << "." << endl << endl;

		// --- 명령어 처리 및 잠금 로직 ---
		if (isChanged && cmd != '4' && cmd != '0') {
			// 4번이 실행된 상태에서는 4번(복구)과 0번(종료)만 가능
			cout << "현재 정렬 모드입니다. 4번을 다시 눌러 복구한 뒤 이용하세요." << endl;
		}
		else {
			if (cmd >= 'a' && cmd <= 'z') {
				for (auto& word : words) {
					for (auto& c : word) {
						if (c == cmd) c = char(toupper(c));
						else if (char(tolower(c)) == cmd) c = char(tolower(c));
					}
				}
			}
			else if (cmd == '1') {
				for (auto& i : spacecnts) if (i > 0) --i;
			}
			else if (cmd == '2') {
				for (auto& i : spacecnts) if (i < 5) ++i;
			}
			else if (cmd == '3') {
				for (int i = 0; i < 58; ++i) alphacnts[i] = 0;
				for (const auto& word : words) {
					for (const auto& c : word) {
						if (c >= 'A' && c <= 'z') ++alphacnts[c - 'A'];
					}
				}
				cout << "오름차순 정렬 : ";
				for (int i = 0; i < 58; ++i) {
					if (alphacnts[i] != 0) cout << char(i + 'A') << alphacnts[i];
				}
				cout << endl << endl;
			}
			else if (cmd == '4') {
				isorder4 = true;
				if (!isChanged) {
					// 1. 현재 순서를 temp에 복사 (백업)
					temp.assign(words.begin(), words.end());
					// 2. 길이순 정렬
					words.sort([](const string& a, const string& b) {
						return a.size() < b.size();
						});
					isChanged = true;
				}
				else {
					// 3. 다시 4번을 누르면 temp에서 원래 순서 복구
					words.assign(temp.begin(), temp.end());
					isChanged = false;
				}

				// 정렬/복구 후 결과 출력
				cout << "변경 후 문장 : ";
				auto ib = words.begin();
				for (int i = 0; i < words.size(); ++i, ++ib) {
					cout << *ib;
					if (i != words.size() - 1) cout << ' ';
				}
				cout << "." << endl << endl << endl << endl;
			}
			else if (cmd == '0') {
				isrunning = false;
			}
			else {
				cout << "명령어를 다시 입력하세요";
				system("pause");
			}
		}

		// --- 일반 명령어 결과 출력 (4번 모드가 아닐 때만) ---
		if (!isorder4 && isrunning)
		{
			cout << "변경 후 문장 : ";
			int currentLen = 0;
			auto ib = words.begin();
			for (int i = 0; i < words.size(); ++i, ++ib) {
				for (char c : *ib) {
					if (currentLen < 40) { cout << c; currentLen++; }
				}
				if (i < spacecnts.size()) {
					for (int j = 0; j < spacecnts[i]; ++j) {
						if (currentLen < 40) { cout << ' '; currentLen++; }
					}
				}
				if (currentLen >= 40) break;
			}
			cout << "." << endl << endl << endl;
		}
		system("pause");
	}
}

void makewords(char* sen)
{
	int cnt = 0;
	string word{};
	int spacecnt = 0;
	char c{};

	for (int i = 0; i < 39; ++i, ++sen) {
		if (*sen == '.' || *sen == '\0') break;
		else if (*sen == ' ' && cnt > 0) {
			if (c != ' ') {
				words.push_back(word);
				word = "";
			}
			++spacecnt;
			c = ' ';
		}
		else if (*sen != ' ') {
			if (c == ' ') {
				spacecnts.push_back(spacecnt);
				spacecnt = 0;
			}
			word += *sen;
			c = *sen;
			++cnt;
		}
	}
	if (!word.empty()) words.push_back(word);
}