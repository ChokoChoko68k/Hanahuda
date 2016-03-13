#include"utility.h"
#include"Choko.h"
#include"Scene.h"

int fonthandle;//externed：非推奨

DATEDATA date;//for DxLib::GetDateTime(DATEDATA*pointer)

void DebugLight();

int WINAPI WinMain(
	HINSTANCE instancehandle,
	HINSTANCE preinstancehandle, //16bit時代の産物;常にNULL
	LPSTR commandline,//ダブルクリックで開いたファイルの名前など．しかし""が入ってしまうというばっきゃろー;Unicodeに対応してないためこれを使うのは非推奨らしい
	int howtoshowwindow) {

	int graphmode = SetGraphMode(SCREEN_HEIGHT, SCREEN_WIDTH, 24);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);

	ChangeWindowMode(TRUE);//全画面もーどじゃない:初期化の前に処理しないと一瞬全画面になる

	SetWindowText("まる見え花札");

	//SetWindowIconHandle(LoadIcon(hI, MAKEINTRESOURCE(IDI_ICON)));

	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() != 0)return -1;

	//if (CursorLoad("graphic/cursor.png"))return -1;

	//create original font : if error, use default font
	fonthandle = CreateFontToHandle("hanazono-20141012/HanaMinA.ttf", 16, -1);
	if (fonthandle <= 0) {
		fonthandle = CreateFontToHandle(NULL, 16, -1);
	}

	SceneManager::CreateInstance();

	SceneManager::GetInstance()->CreateScene(TITLE, DOWNER);

	while (true) {

		SetDrawScreen(DX_SCREEN_BACK);

		ClearDrawScreen();

		InputReceive();

		if (SceneManager::GetInstance()->Update() != 0)break;
		SceneManager::GetInstance()->Draw();

		CursorDraw();

#ifdef _DEBUG
		DebugLight();
#endif

		ScreenFlip();

		//ENDPROCESS
		if (ProcessMessage() != 0)break;
		if (keystate_9 == ON)break;
	}
	DxLib_End();

	return 0;
}

void DebugLight() {
	clsDx();
	printfDx("%ffps\nJoypad数=%d\nmousex=%d\nmousey=%d\nclick_left=%d\n"
		, GetFps(), GetJoypadNum(), mousex, mousey, click_left);
}

/*
new 型名
new 型名[要素数]
new コンストラクタ
*/