#include"utility.h"
#include"TitleScene.h"
#include"Choko.h"

TitleScene::TitleScene() {
	graph_back = LoadGraph("graphic/title.png");
}

TitleScene::~TitleScene() {
}

void TitleScene::Update() {
	if (click_left == 1) {
		if (mousex < SCREEN_WIDTH / 2) {
			SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
			SceneManager::GetInstance()->CreateScene(SceneID::GAME, SceneLayer::DOWNER);
		}
		else {
			SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
			SceneManager::GetInstance()->CreateScene(SceneID::VSCPGAME, SceneLayer::DOWNER);
		}
		return;
	}
}

void TitleScene::Draw() {
	DrawGraph(0, 0, graph_back, FALSE);
	DrawFormatStringToHandle(SCREEN_WIDTH/2 - 100, 500, 0xff1111, fonthandle, "ëŒêÌ");
	DrawFormatStringToHandle(SCREEN_WIDTH/2 + 100, 500, 0xff1111, fonthandle,"ëŒìdî]ëŒêÌ");
	DrawFormatStringToHandle(SCREEN_WIDTH/2, 650, 0xff1111, fonthandle, "èIóπ");
}