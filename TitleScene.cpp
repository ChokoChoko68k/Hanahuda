#include"utility.h"
#include"TitleScene.h"
#include"Choko.h"

TitleScene::TitleScene() {
	graph_back = LoadGraph("graphic/title.png");
}

TitleScene::~TitleScene() {
}

int TitleScene::Update() {
	if (click_left == 1) {
		if (mousex > SCREEN_WIDTH / 2 - 25 && mousex < SCREEN_WIDTH / 2 + 25 && mousey>650 && mousey < 650 + 30) {//End the game
			return -1;
		}
		else if (mousex < SCREEN_WIDTH / 2) {
			SceneManager::GetInstance()->SetIsvscp(false);
			SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
		}
		else {
			SceneManager::GetInstance()->SetIsvscp(true);
			SceneManager::GetInstance()->CreateScene(SceneID::FUSUMA, SceneLayer::UPPER);
		}
	}
	return 0;
}

void TitleScene::Draw() {
	DrawGraph(0, 0, graph_back, FALSE);
	DrawFormatStringToHandle(SCREEN_WIDTH/2 - 100, 500, 0xff1111, fonthandle, "ëŒêÌ");
	DrawFormatStringToHandle(SCREEN_WIDTH/2 + 100, 500, 0xff1111, fonthandle,"ëŒìdî]ëŒêÌ");
	DrawFormatStringToHandle(SCREEN_WIDTH/2 - 20, 650, 0xff1111, fonthandle, "èIóπ");
}