// SS2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <opencv2\opencv.hpp>
#include <MIDIData.h>
int main()
{
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

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
