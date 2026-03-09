#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Showtime {
	string time;
	int seats[10][10] = { 0 };
};

struct Theater {
	int id;               
	string title;         
	Showtime schedules[3];
};

Theater theaters[3];
int globalReservationId = 10; 

void initData() {
	theaters[0] = { 1, "Jaws", {{"1000"}, {"1400"}, {"1800"}} };
	theaters[1] = { 2, "Graduation", {{"1130"}, {"1530"}, {"2130"}} };
	theaters[2] = { 3, "Big", {{"0900"}, {"1300"}, {"1700"}} };
}

// d: 영화관 정보 출력
void displayTheaters() {
	cout << "\n--- Movie Theater Info ---" << endl;
	for (int i = 0; i < 3; ++i) {
		cout << theaters[i].id << " " << theaters[i].title << " ";
		for (int j = 0; j < 3; ++j) cout << theaters[i].schedules[j].time << " ";
		cout << endl;
	}
}

// p: 좌석 상태 출력
void printSeats(string query) {
	for (int i = 0; i < 3; ++i) {
		if (to_string(theaters[i].id) == query || theaters[i].title == query) {
			cout << "\n========================================";
			cout << "\n MOVIE: " << theaters[i].title;
			cout << "\n========================================" << endl;

			for (int s = 0; s < 3; ++s) {
				cout << "\n[TIME: " << theaters[i].schedules[s].time << "]" << endl;
								
				cout << "      ";
				for (int c = 0; c < 10; ++c) cout << setw(2) << c << " ";
				cout << "\n    " << string(32, '-') << endl;

				for (int r = 0; r < 10; ++r) {
					// 행 번호 출력 
					cout << "Row " << r << "| ";
					for (int c = 0; c < 10; ++c) {
						if (theaters[i].schedules[s].seats[r][c] == 0) {
							cout << " -- ";
						}
						else {
							cout << " " << setw(2) << theaters[i].schedules[s].seats[r][c] << " ";
						}
					}
					cout << endl;
				}
				cout << "    " << string(32, '-') << endl;
			}
			return;
		}
	}
	cout << "Theater or Movie not found." << endl;
}

// r: 영화 예약
void reserveMovie() {
	string movieInput, timeInput;
	int row, col;

	cout << "Which Movie (Name or ID): "; cin >> movieInput;
	cout << "What time (e.g., 1000): "; cin >> timeInput;
	cout << "Seat you want (row col): "; cin >> row >> col;

	if (row < 0 || row >= 10 || col < 0 || col >= 10) {
		cout << "Invalid seat position!" << endl;
		return;
	}

	for (int i = 0; i < 3; ++i) {
		if (to_string(theaters[i].id) == movieInput || theaters[i].title == movieInput) {
			for (int s = 0; s < 3; ++s) {
				if (theaters[i].schedules[s].time == timeInput) {
					if (theaters[i].schedules[s].seats[row][col] == 0) {
						theaters[i].schedules[s].seats[row][col] = globalReservationId;
						cout << "\n[DEBUG] Saved in index: Theater[" << i << "], Schedule[" << s << "]" << endl;
						cout << "Reservation Confirmed!" << endl;
						cout << "Your reservation number is " << globalReservationId++ << "." << endl;
							return;
					}
					else {
						cout << "Already reserved seat!" << endl;
						return;
					}
				}
			}
		}
	}
	cout << "Invalid movie or time information." << endl;
}

// c: 예약 취소
void cancelReservation() {
	int resId;
	cout << "Enter the reservation number: "; cin >> resId;

	for (int i = 0; i < 3; ++i) {
		for (int s = 0; s < 3; ++s) {
			for (int r = 0; r < 10; ++r) {
				for (int c = 0; c < 10; ++c) {
					if (theaters[i].schedules[s].seats[r][c] == resId) {
						theaters[i].schedules[s].seats[r][c] = 0;
						cout << "Reservation " << resId << " cancelled. Movie: " << theaters[i].title << endl;
						return;
					}
				}
			}
		}
	}
	cout << "Reservation number not found." << endl;
}

// h: 예약률 출력 
void displayRates() {
	cout << fixed << setprecision(2);
	for (int i = 0; i < 3; ++i) {
		cout << theaters[i].id << " " << theaters[i].title << ": ";
		for (int s = 0; s < 3; ++s) {
			int count = 0;
			for (int r = 0; r < 10; ++r)
				for (int c = 0; c < 10; ++c)
					if (theaters[i].schedules[s].seats[r][c] != 0) count++;

			double rate = (static_cast<double>(count) / 100.0) * 100.0;
			cout << theaters[i].schedules[s].time << ": " << rate << "%  ";
		}
		cout << endl;
	}
}

int main() {
	initData();
	string cmd;

	while (true) {
		cout << "\nCommand (d, p, r, c, h, q): ";
		cin >> cmd;

		if (cmd == "q") break;
		else if (cmd == "d") displayTheaters();
		else if (cmd[0] == 'p') {
			string query;
			if (cmd.length() > 1) query = cmd.substr(1); // p1 형태
			else cin >> query; // p Jaws 형태
			printSeats(query);
		}
		else if (cmd == "r") reserveMovie();
		else if (cmd == "c") cancelReservation();
		else if (cmd == "h") displayRates();
	}
	return 0;
}