#pragma once
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
namespace case1 {
	vector<int> Pitch(vector<int> smallVector, int highestPitch, int lowestPitch);

	vector<int> Place(vector<int> smallVector, int borderValue, int during);

	vector<int> Distance(vector<int> smallVector, int borderValue, int during);
}
