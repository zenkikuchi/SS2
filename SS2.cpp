// SS2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
#include <MIDIData.h>
#include "Case1.h"

typedef struct RgbColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RgbColor; // RGBそれぞれ最大値は255

typedef struct HsvColor
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor; //HSVそれぞれ最大値は255

void RgbToHsv(RgbColor& rgb, HsvColor& hsv)
{
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return;
	}

	if (rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return;
}

struct stImage {
public:
	std::vector<int> R, G, B, H, S, V;
	HsvColor firsthsv;

	void Setup(cv::Mat image) {
		for (int y = 0; y < image.rows; y++) {
			for (int x = 0; x < image.cols; x++) {
				cv::Mat3b reImageAcc = image;
				cv::Vec3b pointer = reImageAcc(cv::Point(x, y));
				int number = image.cols * y + x + 1;

				/*HSVに変換*/
				RgbColor rgb;
				HsvColor hsv;
				rgb.r = pointer[2];
				rgb.g = pointer[1];
				rgb.b = pointer[0];
				RgbToHsv(rgb, hsv);

				/*debug*/
				if (x == 0 && y == 0) {
					firsthsv.h = hsv.h;
					firsthsv.s = hsv.s;
					firsthsv.v = hsv.v;
				}
				/*vectorに代入*/
				R.push_back(pointer[2]);
				G.push_back(pointer[1]);
				B.push_back(pointer[0]);
				H.push_back(hsv.h);
				S.push_back(hsv.s);
				V.push_back(hsv.v);

				//printf("%d, R%d, G%d, B%d, H%d, S%d, V%d\n",sNumber,sMyR,sMyG,sMyB,sMyH,sMyS,sMyV);
				//printf("%d\n",vecR[number-1]);                           //vectorはvenR[0]から始まる
			}
		}
	}
};

int Debug(), Setup();
void Case1(), MovePitch(std::vector<int>);
/*環境変数*/
cv::Mat smallImage;
stImage im;
int smallImageWidth = 10;	//縮小画像の横幅
int smallImageHeight = 10;  //縮小画像の高さ
bool saveSmallImage = true; //縮小画像を保存するかどうか　trueかfalse
std::string filename;
/* 1 : 絵画の音化(オルゴール式)*/

int case1_tempo = 240;				//曲全体のテンポ（BPM）デフォ120　使用していない
float case1_tempo倍率 = 1.5;		//テンポに掛ける倍率　1で等倍

int case1_lowestPitch_H = 60;		//一番低い音　0~127まで
int case1_highestPitch_H = 84;		//一番高い音　0~127まで
int case1_borderValue_H = 15;       //境界と認識する値の幅　0~255
int case1_during_H = 120;			// 次の音までの長さ　120で100％　60で50％の長さ　120を超えると次の音と重なる

int case1_lowestPitch_S = 48;		//一番低い音　0~127まで
int case1_highestPitch_S = 72;		//一番高い音　0~127まで
int case1_borderValue_S = 15;       //境界と認識する値の幅　0~255
int case1_during_S = 120;			// 次の音までの長さ　120で100％　60で50％の長さ　120を超えると次の音と重なる
int main()
{
	/*!------------------------------------------管理---------------------------------------------*/
	for (std::string str = "1"; str == "1";) {
		if (Setup() == 0) {
			std::cout << "キーを入力して終了します\n";
			std::cin;
			return 1;
		}
		printf("使用するパターンを下の番号から選んで入力してください\n");
		printf("1 : 絵画の音化(オルゴール式)\n");

		int selectNumber;
		std::cin >> selectNumber;

		switch (selectNumber)
		{
		case 1:
			Case1();
			break;
		default:
			printf("正しい選択肢が入力されませんでした。\n");
			break;
		}

		printf("他の画像を使用してもう一度音楽を作成したい場合、 1 と入力してください。");
		std::cin >> str;
	}
	/*!--------------------------------------デバッグ用---------------------------------------------*/

	//TestMIDIData();                                                  //MIDIData.hの正常動作確認
	//TestFreeImage();                                                 //FreeImage.hの正常動作確認
	//TestMIDIDataFormat();									    //MIDIFormat1の正常動作確認
	//Debug();
}

int Debug() {
	std::cout << "Hello World!\n";
	MIDIData* pMIDIData;
	MIDITrack* pMIDITrack;
	/* MIDIデータの生成(フォーマット0,トラック数1,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT0, 1, MIDIDATA_TPQNBASE, 120);
	if (pMIDIData == NULL) {
		printf("MIDIデータの生成に失敗しました。\n");
		return 0;
	}
	/* 最初のトラックへのポインタを取得 */
	pMIDITrack = MIDIData_GetFirstTrack(pMIDIData);
	/* イベントを挿入 */
	MIDITrack_InsertTrackName(pMIDITrack, 0, L"ちょうちょ"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack, 0, 60000000 / 120); /* 120BPM */
	MIDITrack_InsertProgramChange(pMIDITrack, 0, 0, 0); /* Piano1 */
	/* ノートイベントを挿入            Time  ch  key  vel  dur */
	MIDITrack_InsertNote(pMIDITrack, 0, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 120, 0, 64, 100, 120); /* み */
	MIDITrack_InsertNote(pMIDITrack, 240, 0, 64, 100, 240); /* みー */
	MIDITrack_InsertNote(pMIDITrack, 480, 0, 65, 100, 120); /* ふぁ */
	MIDITrack_InsertNote(pMIDITrack, 600, 0, 62, 100, 120); /* れ */
	MIDITrack_InsertNote(pMIDITrack, 720, 0, 62, 100, 240); /* れー */
	MIDITrack_InsertNote(pMIDITrack, 960, 0, 60, 100, 120); /* ど */
	MIDITrack_InsertNote(pMIDITrack, 1080, 0, 62, 100, 120); /* れ */
	MIDITrack_InsertNote(pMIDITrack, 1200, 0, 64, 100, 120); /* み */
	MIDITrack_InsertNote(pMIDITrack, 1320, 0, 65, 100, 120); /* ふぁ */
	MIDITrack_InsertNote(pMIDITrack, 1440, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 1560, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 1680, 0, 67, 100, 240); /* そー */
	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack, 1920);
	/* MIDIデータを保存 */
	MIDIData_SaveAsSMF(pMIDIData, L"chocho.mid");
	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;
	cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC3);
	imshow("", image);
	cv::waitKey(0);
	char str;
	std::cin >> str;
}

int Setup() {
	//!Releaseで実行
	//!cv::imreadが動かない致命的な現象あり
	std::string filepath;
	std::cout << "ファイルパスを入力してください\n";
	std::cin >> filepath;
	if (filepath[0] == '"') {
		filepath = filepath.substr(1);
		filepath = filepath.erase(filepath.size() - 1);
	}
	cv::Mat img = cv::imread(filepath);    // カラー(RGBの3チャンネル)で読み込み
	if (img.empty()) {
		std::cout << "画像の読み込みに失敗しました。\n";
		return 0;
	}
	//cv::namedWindow("自由変更可能", cv::WINDOW_FREERATIO);
	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("自由変更可能", im);
	cv::imshow("Image", img);

	int width = img.cols;
	int height = img.rows;
	int test = 0.2;
	cv::Mat showreImage;
	cv::resize(img, smallImage, cv::Size(10, 10));
	//cv::resize(resizedImage, resizedImage, cv::Size(width,height));
	cv::resize(smallImage, showreImage, cv::Size(500, 500), 1, 1, cv::INTER_NEAREST);
	cv::imshow("resize", showreImage);
	//x==========================================
	cv::Mat grayImage;
	cv::cvtColor(img, grayImage, cv::COLOR_BGRA2GRAY);
	cv::imshow("Gray Scale", grayImage);
	//x==========================================
	im.Setup(smallImage);

	cv::waitKey();
	cv::imwrite("ORIGINimage.jpg", showreImage);

	std::cout << "R : " << im.R[0] << " G : " << im.G[0] << " B : " << im.B[0] << std::endl;
	std::cout << "H : " << im.H[0] << " S : " << im.S[0] << " V : " << im.V[0] << std::endl;

	filename = filepath;
	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}
	// Remove extension if present.
	const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx)
	{
		filename.erase(period_idx);
	}
	//ここから画像処理
}

void Case1() {
	MovePitch(im.V);
	std::vector<int> pitch_H = case1::Pitch(im.H, case1_highestPitch_H, case1_lowestPitch_H); //Hから作られるの音の高さを決定
	std::vector<int> place_H = case1::Place(im.H, case1_borderValue_H, case1_during_H);		//Hから作られるの音の鳴る場所を決定
	std::vector<int> distance_H = case1::Distance(im.H, case1_borderValue_H, case1_during_H);  //Hから作られる音の次の音までの長さ

	std::vector<int> pitch_S = case1::Pitch(im.S, case1_highestPitch_S, case1_lowestPitch_S);//Sから作られるの音の高さを決定
	std::vector<int> place_S = case1::Place(im.S, case1_borderValue_S, case1_during_S);		//Sから作られる音の鳴る場所を決定
	std::vector<int> distance_S = case1::Distance(im.S, case1_borderValue_S, case1_during_S);  //Sから作られる音の次の音までの長さ
	/*place_Hについての例外規定*/
	if (place_H.size() == 0) { std::cout << "条件設定の欠陥により、音楽を生成できませんでした。\n"; return; }

	int sumALL = 0;			//H価とS値の合計　テンポの決定に使用
	int case1_lasttempo;	//最終的なテンポ

	for (int i = 0; i < im.H.size(); i++)
	{
		sumALL += im.H[i];
		sumALL += im.S[i];
	}

	sumALL = sumALL / (im.H.size() * 2);

	case1_lasttempo = (sumALL * 180 / 255 + 60) * case1_tempo倍率;
	printf("テンポは%dです\n", case1_lasttempo);
	/*******************************:/
	/*境界と判定した場所を真っ赤にする*/
	std::vector<int> getPlace;
	cv::Mat redImage = smallImage;
	int latestPlace = 0;
	for (unsigned int i = 1; i < im.H.size(); i++) {
		if (abs(im.H[i] - im.H[i - 1]) > case1_borderValue_H || abs(im.H[i] - im.H[i - 1]) > 255 - case1_borderValue_H) {
			getPlace.push_back(1);
			latestPlace = i;
		}
		else{
			getPlace.push_back(0);
		}
	}
	int x, y;
	for (unsigned int i = 1; i < im.H.size(); i++) {
		if (getPlace[i] == 1) {
			x = i % smallImage.cols;
			y = (i - x) / smallImage.cols;
			redImage.at<cv::Vec3b>(x, y)[2] = 255;
			redImage.at<cv::Vec3b>(x, y)[1] = 0;
			redImage.at<cv::Vec3b>(x, y)[0] = 0;
		}
	}
	cv::Mat showredImage;
	cv::resize(redImage, showredImage, cv::Size(500, 500), 1, 1, cv::INTER_NEAREST);
	cv::imshow("redimage", showredImage);
	cv::imwrite("RedImage.jpg", showredImage);
	cv::waitKey(0);
	/*******************************/
	/*!*********************************************************************************************/

	/*実際にMIDIにする*/

	MIDIData* pMIDIData;
	MIDITrack* pMIDITrack0 = 0;
	MIDITrack* pMIDITrack1 = 0;
	MIDITrack* pMIDITrack2 = 0;
	MIDITrack* pMIDITrack3 = 0;

	/* !MIDIデータの生成 (フォーマット1,トラック数4,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT1, 4, MIDIDATA_TPQNBASE, 120);

	if (pMIDIData == NULL) {
		printf("MIDIデータの生成に失敗しました。\n");
	}
	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック0(コンダクタートラック)*/
	/* 最初のトラックへのポインタを取得 */
	pMIDITrack0 = MIDIData_GetFirstTrack(pMIDIData);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack0, 0/*これは絶対時刻*/, L"コンダクタートラック");
	MIDITrack_InsertTempo(pMIDITrack0, 0, 60000000 / case1_lasttempo);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック1 ( Hの値 ) */
	/*二つ目のトラックへのポインタを取得*/
	pMIDITrack1 = MIDITrack_GetNextTrack(pMIDITrack0);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack1, 0, L"H値トラック");
	MIDITrack_InsertProgramChange(pMIDITrack1, 0, 0, 1); /*ナイロンギター*/
	for (unsigned int i = 0; i < place_H.size(); i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack1, place_H[i], 0, pitch_H[i], 127, distance_H[i + 1]);
		//MIDITrack_InsertNote(pMIDITrack1, place_H[i], 0, pitch_H[i]+7, 127, distance_H[i + 1]);
		//MIDITrack_InsertNote(pMIDITrack1, place_H[i], 0, pitch_H[i]+12, 127, distance_H[i + 1]);
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack1, place_H[place_H.size() - 1] + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック2 ( Sの値 ) */
	/*3つ目のトラックへのポインタを取得*/
	pMIDITrack2 = MIDITrack_GetNextTrack(pMIDITrack1);
	MIDITrack_InsertProgramChange(pMIDITrack2, 0, 1, 1); /*ナイロンギター*/

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack2, 0, L"S値トラック");
	for (unsigned int i = 0; i < place_S.size(); i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack2, place_S[i], 1, pitch_S[i], 127, distance_S[i + 1]);
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack2, place_S[place_S.size() - 1] + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック3 (ドラムベーストラック)*/
	/*ドラムベーストラックへのポインタを取得*/
	pMIDITrack3 = MIDITrack_GetNextTrack(pMIDITrack2);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack3, 0, L"ドラムトラック");
	for (int i = 0; i * 120 < place_H[place_H.size() - 1] || i * 120 < place_S[place_S.size() - 1]; i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack3, i * 120, 9, 35, 127, 240);/*35番　アコースティックバスドラム*/
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack3, MIDITrack_GetEndTime(pMIDITrack3) + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/* MIDIデータを保存 */
	setlocale(LC_CTYPE, "jpn");
	std::string midFilename = filename + ".mid";
	const char* midfileChar = midFilename.c_str();
	size_t capacity = strlen(midfileChar) + 1;
	wchar_t* str2 = (wchar_t*)malloc(sizeof(wchar_t) * capacity);

	// char -> wchar_tの変換
	int result = mbstowcs(str2, midfileChar, capacity);
	if (MIDIData_SaveAsSMF(pMIDIData, str2) == 1)printf("MIDIデータの生成に成功しました\n");

	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;
}

void MovePitch(std::vector<int> inputVec) {
	int sumAll = 0;
	for (int i = 0; i < inputVec.size(); i++)
	{
		sumAll += inputVec[i];
	}
	sumAll = sumAll / inputVec.size();
	std::cout << "明度のの平均値は" << sumAll << "です\n";
	if (sumAll >= 132) {
		case1_highestPitch_H += (sumAll - 132) / 4.8;
		case1_lowestPitch_H += (sumAll - 132) / 4.8;
		case1_highestPitch_S += (sumAll - 132) / 4.8;
		case1_lowestPitch_S += (sumAll - 132) / 4.8;
	}
	else {
		case1_highestPitch_H -= (132 - sumAll) / 4.1;
		case1_lowestPitch_H -= (132 - sumAll) / 4.1;
		case1_highestPitch_S -= (132 - sumAll) / 4.1;
		case1_lowestPitch_S -= (132 - sumAll) / 4.1;
	}
}