#include"utility.h"
#include"Choko.h"
#include"Scene.h"

int fonthandle;//externed：非推奨

DATEDATA date;//for DxLib::GetDateTime(DATEDATA*pointer)

void DebugLight();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,//インスタンスハンドル
	_In_opt_ HINSTANCE hPrevInstance,//HINSTANCE preinstancehandle, 16bit時代の産物;常にNULL
	_In_ LPWSTR    lpCmdLine,//LPSTR commandline,//ダブルクリックで開いたファイルの名前など．しかし""が入ってしまうというばっきゃろー;Unicodeに対応してないためこれを使うのは非推奨らしい
	_In_ int       nCmdShow)//int howtoshowwindow
{

	int graphmode = SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);

	ChangeWindowMode(TRUE);//全画面もーどじゃない:初期化の前に処理しないと一瞬全画面になる

	SetWindowText("花札 辛口");

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

	SceneManager::GetInstance()->CreateScene(SceneID::TITLE, SceneLayer::DOWNER);

	while (ProcessMessage() == 0 && keystate_9 != 1) {

		SetDrawScreen(DX_SCREEN_BACK);

		ClearDrawScreen();

		InputReceive();

		if (SceneManager::GetInstance()->Update() != 0)break;
		SceneManager::GetInstance()->Draw();

		//CursorDraw();

#ifdef _DEBUG
		DebugLight();
#endif

		ScreenFlip();
	}
	DxLib_End();

	return 0;
}

void DebugLight() {
	clsDx();
	printfDx("%ffps\nJoypad数=%d\nmousex=%d\nmousey=%d\nclick_left=%d\nkeystate_1=%dn"
		, GetFps(), GetJoypadNum(), mousex, mousey, click_left,keystate_1);
}

/*
new 型名
new 型名[要素数]
new コンストラクタ
*/