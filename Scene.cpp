#include"Scene.h"
#include"TitleScene.h"
#include"FusumaScene.h"
#include"GameScene.h"
#include"VSCPGameScene.h"
#include"ResultScene.h"

//静的メンバ変数の実体宣言
SceneManager* SceneManager::pointer_instance;//無いとリンカエラーが出る

int SceneManager::Update() {

	int result_upper = -1;
	int result_downer = -1;

	//Scene Update
	if (upperscene != nullptr)result_upper = upperscene->Update();
	else if (downerscene != nullptr)result_downer = downerscene->Update();

	//Null Pointer Error
	if (upperscene == nullptr && downerscene == nullptr)return -1;

	//Check Update Result
	if (result_upper != 0 && result_downer != 0)return -1;
	else return 0;
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

	case SceneID::VSCPGAME:
		if (_layer == SceneLayer::DOWNER)downerscene = new VSCPGameScene(gamenum, teban, score_player0, score_player1);
		else if (_layer == SceneLayer::UPPER)upperscene = new VSCPGameScene(gamenum, teban, score_player0, score_player1);
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