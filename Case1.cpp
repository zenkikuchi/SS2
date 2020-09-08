#include "Case1.h"
namespace case1 {
	vector<int> Pitch(vector<int> smallVector, int highestPitch, int lowestPitch) {
		vector<int> returnPitch;
		int pitchWeight = highestPitch - lowestPitch;
		int insertedPitch;

		for (unsigned int i = 0; i < smallVector.size(); i++) {
			insertedPitch = (int)roundf(smallVector[i] * pitchWeight / 255 + lowestPitch);
			returnPitch.push_back(insertedPitch);
		}
		return returnPitch;
	}

	vector<int> Place(vector<int> smallVector, int borderValue, int during) {
		vector<int> returnPlace;
		vector<int> returnDistance;

		int latestPlace = 0;
		std::cout << "ベクトルのサイズは" << smallVector.size() << "です";
		for (unsigned int i = 1; i < smallVector.size(); i++) {
			if (abs(smallVector[i] - smallVector[i - 1]) > borderValue || abs(smallVector[i] - smallVector[i - 1]) > 255 - borderValue) {
				returnPlace.push_back(i * 120);
				returnDistance.push_back((i - latestPlace) * during);
				latestPlace = i;
			}
		}
		if (latestPlace == 0) {
			std::cout << "色相値の変化量が少なすぎるため、Hから値を抽出できませんでした\n";
		}
		returnDistance.push_back(60);

		return returnPlace;
	}

	vector<int> Distance(vector<int> smallVector, int borderValue, int during) {
		vector<int> returnPlace;
		vector<int> returnDistance;

		int latestPlace = 0;

		for (unsigned int i = 1; i < smallVector.size(); i++) {
			if (abs(smallVector[i] - smallVector[i - 1]) > borderValue || abs(smallVector[i] - smallVector[i - 1]) > 255 - borderValue) {
				returnPlace.push_back(i * 120);
				returnDistance.push_back((i - latestPlace) * during);
				latestPlace = i;
			}
		}
		returnDistance.push_back(120);

		return returnDistance;
	}
}