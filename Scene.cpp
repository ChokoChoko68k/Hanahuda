#include"Scene.h"
#include"TitleScene.h"
#include"FusumaScene.h"
#include"GameScene.h"
#include"ResultScene.h"

//Ã“Iƒƒ“ƒo•Ï”‚ÌŽÀ‘ÌéŒ¾
SceneManager* SceneManager::pointer_instance;

int SceneManager::Update() {

	if (upperscene != nullptr)upperscene->Update();
	else if (downerscene != nullptr)downerscene->Update();

	if (upperscene == nullptr && downerscene == nullptr)return -1;

	return 0;
};

void SceneManager::Draw() {
	if (downerscene != nullptr)downerscene->Draw();
	if (upperscene != nullptr)upperscene->Draw();
};

void SceneManager::CreateScene(SceneID _sceneid, SceneLayer _layer, int _gamenum, int _teban, int _score_player0, int _score_player1) {

	gamenum = _gamenum;
	teban = _teban;
	score_player0 = _score_player0;
	score_player1 = _score_player1;

	DeleteScene(_layer);

	switch (_sceneid) {
	case SceneID::TITLE:
		if (_layer == SceneLayer::DOWNER)downerscene = new TitleScene();
		else if (_layer == SceneLayer::UPPER)upperscene = new TitleScene();
		break;

	case SceneID::FUSUMA:
		if (_layer == SceneLayer::DOWNER)downerscene = new FusumaScene();
		else if (_layer == SceneLayer::UPPER)upperscene = new FusumaScene();
		break;

	case SceneID::GAME:
		if (_layer == SceneLayer::DOWNER)downerscene = new GameScene(gamenum, teban, score_player0, score_player1);
		else if (_layer == SceneLayer::UPPER)upperscene = new GameScene(gamenum, teban, score_player0, score_player1);
		break;

	case SceneID::RESULT:
		if (_layer == SceneLayer::DOWNER)downerscene = new ResultScene(gamenum, teban, score_player0, score_player1);
		else if (_layer == SceneLayer::UPPER)upperscene = new ResultScene(gamenum, teban, score_player0, score_player1);
		break;
	}
}

void SceneManager::DeleteScene(SceneLayer _layer) {
	if (_layer == SceneLayer::DOWNER) {
		if (downerscene != nullptr) {
			delete downerscene;
			downerscene = nullptr;
		}
	}
	else if (_layer == SceneLayer::UPPER) {
		if (upperscene != nullptr) {
			delete upperscene;
			upperscene = nullptr;
		}
	}
}