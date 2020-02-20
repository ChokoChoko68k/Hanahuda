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
		SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
		return;
	}
}

void TitleScene::Draw() {
	DrawGraph(0, 0, graph_back, FALSE);
	DrawFormatStringToHandle(500, 550, 0xff1111, fonthandle, "‘Îí");
	DrawFormatStringToHandle(500, 600, 0xff1111, fonthandle,"‘Î“d”]‘Îí");
}