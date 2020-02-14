#include"utility.h"
#include"FusumaScene.h"
#include"Choko.h"

FusumaScene::FusumaScene() {
	count = 0;
	leftx_limit = 0;
	rightx_limit = SCREEN_WIDTH;

	isclosing = true;

	waitcount = 0;
};

FusumaScene::~FusumaScene() {
};

void FusumaScene::Update() {

	leftx_limit = count;
	rightx_limit = SCREEN_WIDTH - count;

	if (count == SCREEN_WIDTH / 2) {
		if (waitcount == 48) {
			SceneManager::GetInstance()->CreateScene(SceneID::VSCPGAME, SceneLayer::DOWNER);
			SceneManager::GetInstance()->isvscp = true;
			isclosing = false;
		}
		waitcount++;
	}

	if (count < SCREEN_WIDTH / 2) {
		if (isclosing)count += 16;//shutterring speed
	}
	if (count <= SCREEN_WIDTH / 2) {
		if (!isclosing)count -= 16;//shutterring speed
	}


	//‚Ó‚·‚Ü‘S•”ŠJ‚¢‚½
	if (!isclosing && count == 0) {
		SceneManager::GetInstance()->DeleteScene(SceneLayer::UPPER);
		return;
	}
}

void FusumaScene::Draw() {
	DrawBox(0, 0, leftx_limit, SCREEN_HEIGHT, WHITE, TRUE);
	DrawBox(SCREEN_WIDTH, 0, rightx_limit, SCREEN_HEIGHT, WHITE, TRUE);
}