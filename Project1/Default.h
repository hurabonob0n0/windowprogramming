#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 g(rd());
uniform_int_distribution<> dis(0, 9);