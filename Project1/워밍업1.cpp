#include <iostream>
#include <string>
#include <list>
#include <vector>
using namespace std;

bool finished = false;
char order{};
char sentence[40];

void order_finish()
{
	finished = true;
}

void order_a_z() 
{
	for (int i = 0; i < 40; ++i) {
		if (sentence[i] == order) {
			if (sentence[i] >= 'a')
				sentence[i] -= 32;
			else
				sentence[i] += 32;
		}
	}
	cout << "입력한 문장 : " << sentence << endl;
	cout << "바뀐 문장 : " << sentence << endl;
}

void order_1()
{
	for (int i = 0; i < 40; ++i) {
		if (sentence[i] == ' ' && sentence[i + 1] != ' ') {
			for (int j = i; j < 40 - i; ++j) {
				sentence[j] = sentence[j + 1];
			}
			--i;
		}
	}
	cout << "입력한 문장 : " << sentence << endl;
	cout << "바뀐 문장 : " << sentence << endl;
}

void order_2() 
{

}

void frame()
{
	if (order >= 65)
		order_a_z();
	else if (order == '0')
		order_finish();
	else if (order == '1')
		order_1();
	else if (order == '2')
		order_2();
}

int main()
{	
	cout << "문장을 입력하시오 : ";
	cin.getline(sentence, 40);

	while (!finished) {
		cout << "명령어를 입력하시오 : ";
		cin >> order;
		frame();
	}
}