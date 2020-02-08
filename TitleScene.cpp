#include"utility.h"
#include"TitleScene.h"
#include"Choko.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Update() {
	if (click_left == 1) {
		SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
		return;
	}
}

void TitleScene::Draw() {
	DrawFormatStringToHandle(300, 200, 220, fonthandle,"‚Ü‚éŒ©‚¦‰ÔŽD");
}