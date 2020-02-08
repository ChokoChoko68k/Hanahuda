#include"utility.h"
#include"FusumaScene.h"
#include"Choko.h"

FusumaScene::FusumaScene() {
	count = 0;
	leftx_limit = 0;
	rightx_limit = 640;

	isclosing = true;

	waitcount = 0;
};

FusumaScene::~FusumaScene() {
};

void FusumaScene::Update() {

	leftx_limit = count;
	rightx_limit = 640 - count;

	if (count == 320) {
		if (waitcount == 60) {
			SceneManager::GetInstance()->CreateScene(SceneID::GAME, SceneLayer::DOWNER);
			isclosing = false;
		}
		waitcount++;
	}

	if (count < 320) {
		if (isclosing)count += 8;
	}
	if (count <= 320) {
		if (!isclosing)count -= 8;
	}


	//ふすま全部開いた
	if (!isclosing && count == 0) {
		SceneManager::GetInstance()->DeleteScene(SceneLayer::UPPER);
		return;
	}
}

void FusumaScene::Draw() {
	DrawBox(0, 0, leftx_limit, SCREEN_HEIGHT, WHITE, TRUE);
	DrawBox(640, 0, rightx_limit, SCREEN_HEIGHT, WHITE, TRUE);
}

/*
gamenumもスコアもきえちゃう
*/