// SS2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <MIDIData.h>

int Debug();

int main()
{
	//!絶対にDebugモードで実行してはいない
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
	}
	//cv::namedWindow("自由変更可能", cv::WINDOW_FREERATIO);
	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("自由変更可能", im);
	cv::imshow("Image", img);


	int width = img.cols;
	int height = img.rows;
	int test = 0.2;
	cv::Mat reImage;
	cv::Mat showreImage;
	cv::resize(img, reImage, cv::Size(10, 10));
	//cv::resize(resizedImage, resizedImage, cv::Size(width,height));
	cv::resize(reImage, showreImage, cv::Size(500,500),1,1,cv::INTER_NEAREST);
	cv::imshow("resize", showreImage);
	//x==========================================
	cv::Mat grayImage;
	cv::cvtColor(img,grayImage,cv::COLOR_BGRA2GRAY);
	cv::imshow("Gray Scale", grayImage);
	//x==========================================
	cv::waitKey();

	cv::Mat3b reImageAcc = reImage;
	cv::Vec3b pointer = reImageAcc(cv::Point(0,0));
	std::cout << "R : " << (int)pointer[2] << " G : " << (int)pointer[1] << " B : " << (int)pointer[0] << std::endl;
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

