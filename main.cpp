#include"utility.h"
#include"Choko.h"
#include"Scene.h"

int fonthandle;//externed�F�񐄏�

DATEDATA date;//for DxLib::GetDateTime(DATEDATA*pointer)

void DebugLight();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,//�C���X�^���X�n���h��
	_In_opt_ HINSTANCE hPrevInstance,//HINSTANCE preinstancehandle, 16bit����̎Y��;���NULL
	_In_ LPWSTR    lpCmdLine,//LPSTR commandline,//�_�u���N���b�N�ŊJ�����t�@�C���̖��O�ȂǁD������""�������Ă��܂��Ƃ����΂������[;Unicode�ɑΉ����ĂȂ����߂�����g���͔̂񐄏��炵��
	_In_ int       nCmdShow)//int howtoshowwindow
{

	int graphmode = SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);

	ChangeWindowMode(TRUE);//�S��ʂ��[�ǂ���Ȃ�:�������̑O�ɏ������Ȃ��ƈ�u�S��ʂɂȂ�

	SetWindowText("�ԎD �h��");

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
	printfDx("%ffps\nJoypad��=%d\nmousex=%d\nmousey=%d\nclick_left=%d\nkeystate_1=%dn"
		, GetFps(), GetJoypadNum(), mousex, mousey, click_left,keystate_1);
}

/*
new �^��
new �^��[�v�f��]
new �R���X�g���N�^
*/