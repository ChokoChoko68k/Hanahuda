#include"utility.h"
#include"ResultScene.h"
#include"Choko.h"

ResultScene::ResultScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {
	graph_fusuma_left = LoadGraph("graphic/Fusuma_left.jpg");
	graph_fusuma_right = LoadGraph("graphic/Fusuma_right.jpg");

	count = 0;
	leftx_limit = 0;
	rightx_limit = SCREEN_WIDTH;

	isclosing = true;

	gamenum = _gamenum;
	teban = _teban;
	score_player0 = _score_player0;
	score_player1 = _score_player1;

	winner = -1;
	//6���or������I�����H
	if (gamenum >= 1 && score_player0 != score_player1) {//now,2-win-game for debugging
		if (score_player0 > score_player1)winner = 0;
		if (score_player0 < score_player1)winner = 1;
	}
};

ResultScene::~ResultScene() {
};

int ResultScene::Update() {

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
	

	//�ӂ��ܑS���J����
	if (!isclosing && count == 0) {
		SceneManager::GetInstance()->DeleteScene(SceneLayer::UPPER);
		return 0;
	}
	return 0;
}

void ResultScene::Draw() {
	DrawGraph(-SCREEN_WIDTH / 2 + leftx_limit, 0, graph_fusuma_left, FALSE);
	DrawGraph(rightx_limit, 0, graph_fusuma_right, FALSE);
	if (count == SCREEN_WIDTH / 2) {
		DrawExtendFormatString(SCREEN_WIDTH / 2 - 180, 180, 2.0, 2.0, KINAKAJIS, "%d���", gamenum);
		DrawExtendFormatString(SCREEN_WIDTH / 2 - 180, 230, 2.0, 2.0, KINAKAJIS, "%s�F    %d�_", "���Z�҂P", score_player0);
		DrawExtendFormatString(SCREEN_WIDTH / 2 - 180, 280, 2.0, 2.0, KINAKAJIS, "%s�F    %d�_", "���Z�҂Q", score_player1);
		if (winner != -1)DrawExtendFormatString(SCREEN_WIDTH / 2 - 180, 420, 3.0, 3.0, KINAKAJIS, "%d�̏���", winner);
	}
}

/*
gamenum���X�R�A���������Ⴄ
Across Scene Data��ǉ�
*/