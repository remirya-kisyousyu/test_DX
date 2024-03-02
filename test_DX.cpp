// test_DX.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "DxLib.h"
#include "time.h"

#define screenSizeX 640 //横スクリーンサイズ
#define screenSizeY 480 //縦スクリーンサイズ

#define stepSizeMouse 5 //ネズミの移動量
#define stepSizeCat 4 //猫の移動量

#define getCheeseRange 45 //チーズの取得範囲
#define getMouseRange 20 //ネズミの捕獲範囲

//猫の移動処理
void MoveCat(int* catCoordX, int* catCoordY, int mouseCoordX, int mouseCoordY) {
	double disXY = abs(*catCoordX - mouseCoordX) + abs(*catCoordY - mouseCoordY); //X, Yの合計距離

	//X, Yの移動割合
	double moveX = (*catCoordX - mouseCoordX) / disXY;
	double moveY = (*catCoordY - mouseCoordY) / disXY;

	//X, Yの移動量
	*catCoordX -= moveX * stepSizeCat;
	*catCoordY -= moveY * stepSizeCat;
}

//チーズの取得判定
int IsGetCheese(int mouseCoordX, int mouseCoordY, int cheeseCoordX, int cheeseCoordY) {
	if (abs(mouseCoordX - cheeseCoordX) < getCheeseRange && abs(mouseCoordY - cheeseCoordY) < getCheeseRange) {
		return 1; //再生成
	}
	else {
		return 0;
	}
}

//ネズミの捕獲判定（ゲームオーバー判定）
int IsGetMouse(int mouseCoordX, int mouseCoordY, int catCoordX, int catCoordY) {
	if (abs(mouseCoordX - catCoordX) < getMouseRange && abs(mouseCoordY - catCoordY) < getMouseRange) {
		return 1; //ゲームオーバー
	}
	else {
		return 0;
	}
}

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

	cheeseCoordX = rand() % (screenSizeX - sizeCheeseX);
	cheeseCoordY = rand() % (screenSizeY - sizeCheeseY);

	//文字列の表示
	int charaColor;
	charaColor = GetColor(255, 255, 255); //白

	DrawFormatString(screenSizeX / 2 - 60, screenSizeY / 2, charaColor,  "PRESS ANY KEY");
	ScreenFlip();

	//ゲームスタート待機
	WaitKey();

	int currPoint = 0;

	while (TRUE) {
		//スクリーンクリア
		ClearDrawScreen();

		//猫の移動処理
		MoveCat(&catCoordX, &catCoordY, mouseCoordX, mouseCoordY);

		//ネズミの移動処理
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			//左移動
			mouseCoordX -= stepSizeMouse;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			//右移動
			mouseCoordX += stepSizeMouse;
		}
		else if(CheckHitKey(KEY_INPUT_UP) == 1) {
			//上移動
			mouseCoordY -= stepSizeMouse;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			//下移動
			mouseCoordY += stepSizeMouse;
		}
		
		//スクリーン外に出ないようチェック
		if (mouseCoordX < 0) {
			mouseCoordX = 0;
		}
		else if (mouseCoordX > screenSizeX - sizeMouseX) {
			mouseCoordX = screenSizeX - sizeMouseX;
		}

		if (mouseCoordY < 0) {
			mouseCoordY = 0;
		}
		else if (mouseCoordY > screenSizeY - sizeMouseY) {
			mouseCoordY = screenSizeY - sizeMouseY;
		}
		
		//ネズミの捕獲判定
		int getMouseFlag = 0;
		getMouseFlag = IsGetMouse(mouseCoordX, mouseCoordY, catCoordX, catCoordY);

		//ゲームオーバー
		if (getMouseFlag == 1) {
			//ゲームオーバー表示
			DrawFormatString(screenSizeX / 2 - 60, screenSizeY / 2, charaColor, "ゲームオーバー\n得点：%d\n\nPRESS ANY KEY", currPoint);
			ScreenFlip();

			//ゲーム終了
			WaitKey();
			break;
		}

		//チーズの取得判定
		int getCheeseFlag = 0;
		getCheeseFlag = IsGetCheese(mouseCoordX, mouseCoordY, cheeseCoordX, cheeseCoordY);

		//チーズの再生成
		if (getCheeseFlag == 1) {
			currPoint++;
			cheeseCoordX = rand() % (screenSizeX - sizeCheeseX);
			cheeseCoordY = rand() % (screenSizeY - sizeCheeseY);
		}

		//文字列の表示
		DrawFormatString(0, screenSizeY - 60, charaColor, "得点：%d", currPoint);

		//画像の表示
		DrawGraph(mouseCoordX, mouseCoordY, mouse, TRUE);
		DrawGraph(catCoordX, catCoordY, cat, TRUE);
		DrawGraph(cheeseCoordX, cheeseCoordY, cheese, TRUE);

		//スクリーンをコピー
		ScreenFlip();

		//速度制限
		WaitTimer(20);

		//windowsメッセージ処理
		if (ProcessMessage() == -1) break;

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

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
