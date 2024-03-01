// test_DX.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "DxLib.h"
#include "time.h"

#define screenSizeX 640
#define screenSizeY 480
#define stepSize 10

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	//全画面表示の終了
	ChangeWindowMode(TRUE);

	//乱数生成用
	srand((unsigned int)time(NULL));
	
	//DXライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	//画面の設定
	SetGraphMode(screenSizeX, screenSizeY, 16);

	//描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//キャラクター画像、座標、画像サイズ
	int mouse, mouseCoordX, mouseCoordY, sizeMouseX, sizeMouseY;
	int cat, catCoordX, catCoordY, sizeCatX, sizeCatY;
	int cheese, cheeseCoordX, cheeseCoordY, sizeCheeseX, sizeCheeseY;

	//画像をメモリにロード
	mouse = LoadGraph("ネズミ.jpg");
	cat = LoadGraph("猫.jpg");
	cheese = LoadGraph("チーズ.jpg");

	//画像サイズを取得
	GetGraphSize(mouse, &sizeMouseX, &sizeMouseY);
	GetGraphSize(cat, &sizeCatX, &sizeCatY);
	GetGraphSize(cheese, &sizeCheeseX, &sizeCheeseY);

	//座標初期化
	mouseCoordX = 0;
	mouseCoordY = 0;

	catCoordX = screenSizeX - sizeCatX;
	catCoordY = screenSizeY - sizeCatY;

	cheeseCoordX = rand() % screenSizeX;
	cheeseCoordY = rand() % screenSizeY;

	//画像の表示
	DrawGraph(mouseCoordX, mouseCoordY, mouse, FALSE);
	DrawGraph(catCoordX, catCoordY, cat, FALSE);
	DrawGraph(cheeseCoordX, cheeseCoordY, cheese, FALSE);

	//flip
	ScreenFlip();

	WaitKey();

	DxLib_End();

	return 0;
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
