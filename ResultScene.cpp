#include"utility.h"
#include"ResultScene.h"
#include"Choko.h"

ResultScene::ResultScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {
	count = 0;
	leftx_limit = 0;
	rightx_limit = SCREEN_WIDTH;

	isclosing = true;

	gamenum = _gamenum;
	teban = _teban;
	score_player0 = _score_player0;
	score_player1 = _score_player1;

	winner = -1;
	//6回戦or延長戦終了か？
	if (gamenum >= 6 && score_player0 != score_player1) {//now,2-win-game for debugging
		if (score_player0 > score_player1)winner = 0;
		if (score_player0 < score_player1)winner = 1;
	}
};

ResultScene::~ResultScene() {
};

void ResultScene::Update() {

	leftx_limit = count;
	rightx_limit = SCREEN_WIDTH - count;

	if (click_left == 1  && count == SCREEN_WIDTH/2) {
		if (winner != -1) {
			SceneManager::GetInstance()->CreateScene(SceneID::TITLE, SceneLayer::DOWNER);
			isclosing = false;
		}
		else {
			if(SceneManager::GetInstance()->isvscp)SceneManager::GetInstance()->CreateScene(SceneID::VSCPGAME, SceneLayer::DOWNER, gamenum, teban, score_player0, score_player1);
			else SceneManager::GetInstance()->CreateScene(SceneID::GAME, SceneLayer::DOWNER, gamenum, teban, score_player0, score_player1);

			isclosing = false;
		}
	}

	if (count < SCREEN_WIDTH / 2) {
		if (isclosing)count += 5;
	}
	if (count <= SCREEN_WIDTH / 2) {
		if (!isclosing)count -= 5;
	}
	

	//ふすま全部開いた
	if (!isclosing && count == 0) {
		SceneManager::GetInstance()->DeleteScene(SceneLayer::UPPER);
		return;
	}
}

void ResultScene::Draw() {
	DrawBox(0, 0, leftx_limit, SCREEN_HEIGHT, BLACK, TRUE);
	DrawBox(SCREEN_WIDTH, 0, rightx_limit, SCREEN_HEIGHT, BLACK, TRUE);
	if (count == SCREEN_WIDTH / 2) {
		DrawExtendFormatString(SCREEN_WIDTH / 2 - 200, 100, 1.0, 1.0, WHITE, "%d回戦　%d：%d点　%d：%d点", gamenum, 0, score_player0, 1, score_player1);
		if (winner != -1)DrawExtendFormatString(SCREEN_WIDTH / 2 -200, 200, 1.0, 1.0, 0x5af59, "%dの勝ち", winner);
	}
}

/*
gamenumもスコアもきえちゃう
Across Scene Dataを追加
*/